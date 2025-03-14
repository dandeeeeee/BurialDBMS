#include "ResourceManager.hpp"
#include "cpr/cpr.h"
#include <fstream>
#include <iostream>

using namespace std::chrono;

#include "ResourceManager.hpp"
#include "cpr/cpr.h"
#include <fstream>
#include <iostream>

using namespace std::chrono;

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

ResourceManager::ResourceManager() {
    LoadConfig();
    if (!fs::exists(CACHE_DIR)) {
        fs::create_directory(CACHE_DIR);
    }

    running = true;
    workerThread = std::thread(&ResourceManager::ImageLoadingThread, this);
}

ResourceManager::~ResourceManager() {
    running = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }

    for (auto& pair : textureCache) {
        UnloadTexture(pair.second);
    }
    for (auto& pair : fontCache) {
        UnloadFont(pair.second);
    }
}

bool ResourceManager::DownloadFile(const std::string& url, const std::string& outputPath) {
    cpr::Response response = cpr::Get(cpr::Url{url});
    if (response.status_code == 200) {
        std::ofstream file(outputPath, std::ios::binary);
        file.write(response.text.c_str(), response.text.size());
        file.close();
        return true;
    }
    return false;
}

void ResourceManager::LoadFonts() {
    // List of fonts to load (you can extend this list as needed)
    std::vector<std::string> fontFiles = {
        "LeagueSpartan-Bold.ttf",
        "Poppins-Medium.ttf",
        "Poppins-MediumItalic.ttf"
    };

    for (const auto& fontFile : fontFiles) {
        std::string cachePath = CACHE_DIR + "/" + fontFile;
        std::string url = "http://127.0.0.1:8000/fonts/" + fontFile;

        // Download the font if it doesn't exist or is expired
        if (FileExists(cachePath)) {
            if (IsFileExpired(cachePath)) {
                fs::remove(cachePath);
            }
        }

        if (!FileExists(cachePath)) {
            if (!DownloadFile(url, cachePath)) {
                std::cerr << "Failed to download font: " << fontFile << std::endl;
                continue;
            }
        }

        // Load the font
        Font font = LoadFontEx(cachePath.c_str(), 320, nullptr, 0);

        // Check if the font is valid
        if (font.glyphs != nullptr && font.baseSize > 0) {
            // Cache the font
            fontCache[fontFile] = font;
            std::cout << "Loaded font: " << fontFile << std::endl;
        } else {
            std::cerr << "Failed to load font: " << fontFile << std::endl;
            UnloadFont(font); // Unload the invalid font
        }
    }
}


Font ResourceManager::GetFont(const std::string& filename) {
    if (fontCache.find(filename) != fontCache.end()) {
        return fontCache[filename];
    } else {
        std::cerr << "Font not found: " << filename << std::endl;
        return Font();
    }
    return Font();
}

void ResourceManager::LoadConfig() {
    std::ifstream config("config.ini");
    if (config.is_open()) {
        config >> CACHE_EXPIRATION_DAYS;
        config.close();
        printf("Cache expiration set to %d days\n", CACHE_EXPIRATION_DAYS);
    } else {
        printf("Config file not found, using default expiration: %d days\n", CACHE_EXPIRATION_DAYS);
    }
}

bool ResourceManager::FileExists(const std::string& path) {
    return fs::exists(path);
}

bool ResourceManager::IsFileExpired(const std::string& path) {
    auto lastWriteTime = fs::last_write_time(path);
    auto fileAge = duration_cast<hours>(system_clock::now().time_since_epoch()) -
                   duration_cast<hours>(lastWriteTime.time_since_epoch());
    return fileAge.count() > (CACHE_EXPIRATION_DAYS * 24);
}

bool ResourceManager::DownloadImage(const std::string& url, const std::string& outputPath) {
    cpr::Response response = cpr::Get(cpr::Url{url});
    if (response.status_code == 200) {
        std::ofstream file(outputPath, std::ios::binary);
        file.write(response.text.c_str(), response.text.size());
        file.close();
        return true;
    }
    return false;
}

void ResourceManager::LoadImageAsync(const std::string& url, const std::string& cachePath, const std::string& scene) {
    if (FileExists(cachePath)) {
        if (IsFileExpired(cachePath)) {
            fs::remove(cachePath);
        } else {
            std::lock_guard<std::mutex> lock(queueMutex);
            imageQueue.emplace(url, cachePath, scene);
            return;
        }
    }

    if (DownloadImage(url, cachePath)) {
        std::lock_guard<std::mutex> lock(queueMutex);
        imageQueue.emplace(url, cachePath, scene);
    }
}

void ResourceManager::ImageLoadingThread() {
    while (running) {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (!imageQueue.empty()) {
            auto [url, cachePath, scene] = imageQueue.front();
            imageQueue.pop();
            lock.unlock();

            Image img = LoadImage(cachePath.c_str());
            if (img.data != nullptr) {
                std::lock_guard<std::mutex> pendingLock(queueMutex);
                pendingImages.emplace(url, img, scene);
            }
        } else {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

void ResourceManager::ProcessPendingImages() {
    std::lock_guard<std::mutex> lock(queueMutex);
    while (!pendingImages.empty()) {
        auto [url, img, scene] = pendingImages.front();
        pendingImages.pop();

        Texture2D texture = LoadTextureFromImage(img);
        UnloadImage(img);

        std::lock_guard<std::mutex> resourceLock(mutex);
        textureCache[url] = texture;
        textureRefCount[url] = 1;
        textureSceneMap[url] = scene;
        activeSceneTextures.insert(url);
        loadingTextures.erase(url); 
    }
}

Texture2D ResourceManager::GetTexture(const std::string& filename, const std::string& scene) {
    std::string cachePath = CACHE_DIR + "/" + filename;
    std::string url = "http://127.0.0.1:8000/images/" + filename;

    std::lock_guard<std::mutex> lock(mutex);

    // Check if the texture is already cached
    if (textureCache.find(url) != textureCache.end()) {
        textureRefCount[url]++;
        return textureCache[url];
    }

    // Check if the texture is already being loaded
    if (loadingTextures.find(url) != loadingTextures.end()) {
        return Texture2D();
    }

    // Mark the texture as being loaded
    loadingTextures.insert(url);

    // Start an asynchronous load
    std::thread imageLoader(&ResourceManager::LoadImageAsync, this, url, cachePath, scene);
    imageLoader.detach();

    return Texture2D(); 
}


void ResourceManager::ReleaseTexture(const std::string& url) {
    std::lock_guard<std::mutex> lock(mutex);
    if (textureRefCount.find(url) != textureRefCount.end()) {
        textureRefCount[url]--;
        if (textureRefCount[url] <= 0) {
            UnloadTexture(textureCache[url]);
            textureCache.erase(url);
            textureRefCount.erase(url);
            textureSceneMap.erase(url);
            activeSceneTextures.erase(url);
        }
    }
}

void ResourceManager::SwitchScene(const std::string& newScene) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto it = textureCache.begin(); it != textureCache.end();) {
        if (textureSceneMap[it->first] != newScene) {
            UnloadTexture(it->second);
            textureRefCount.erase(it->first);
            textureSceneMap.erase(it->first);
            activeSceneTextures.erase(it->first);
            it = textureCache.erase(it);
        } else {
            ++it;
        }
    }
    currentScene = newScene;
}
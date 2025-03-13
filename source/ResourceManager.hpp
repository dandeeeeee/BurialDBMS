#pragma once

#include "raylib.h"

#if defined(_WIN32)
    #define NOGDI
    #define NOUSER


#endif

#include <Windows.h>

#if defined(_WIN32)
    #undef far

#endif

#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <atomic>
#include <chrono>

namespace fs = std::filesystem;


class ResourceManager {
public:
    static ResourceManager& GetInstance();
    ~ResourceManager();

    Texture2D GetTexture(const std::string& url, const std::string& filename, const std::string& scene);
    void ReleaseTexture(const std::string& url);
    void SwitchScene(const std::string& newScene);
    void ProcessPendingImages(); // Call this in the main thread to process pending textures

private:
    ResourceManager();
    void LoadConfig();
    bool FileExists(const std::string& path);
    bool IsFileExpired(const std::string& path);
    bool DownloadImage(const std::string& url, const std::string& outputPath);
    void LoadImageAsync(const std::string& url, const std::string& cachePath, const std::string& scene);

    void ImageLoadingThread(); // Background thread for loading images

    std::mutex mutex; // Protects textureCache, textureRefCount, etc.
    std::mutex queueMutex; // Protects imageQueue and pendingImages
    std::unordered_map<std::string, Texture2D> textureCache; // Cache for textures
    std::unordered_map<std::string, int> textureRefCount; // Reference counts for textures
    std::unordered_map<std::string, std::string> textureSceneMap; // Maps textures to scenes
    std::unordered_set<std::string> activeSceneTextures; // Textures used in the current scene

    std::queue<std::tuple<std::string, std::string, std::string>> imageQueue; // Queue for images to be loaded
    std::queue<std::tuple<std::string, Image, std::string>> pendingImages; // Queue for images ready to be converted to textures

    std::atomic<bool> running; // Controls the background thread
    std::thread workerThread; // Background thread for loading images

    std::string currentScene;
    int CACHE_EXPIRATION_DAYS = 7;
    const std::string CACHE_DIR = "cache";
};
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


class ResourceManager 
{
    public:
        static ResourceManager& GetInstance();
        ~ResourceManager();
    
        Texture2D GetTexture(const std::string& filename, const std::string& scene);
        void ReleaseTexture(const std::string& url);
        void SwitchScene(const std::string& newScene);
        void ProcessPendingImages(); 
        void LoadFonts();
        Font GetFont(const std::string& filename);
    
    private:
        ResourceManager();
        void LoadConfig();
        bool FileExists(const std::string& path);
        bool IsFileExpired(const std::string& path);
        bool DownloadFile(const std::string& url, const std::string& outputPath); 
        bool DownloadImage(const std::string& url, const std::string& outputPath);
        void LoadImageAsync(const std::string& url, const std::string& cachePath, const std::string& scene);
        void ImageLoadingThread(); 
    
        std::mutex mutex;
        std::mutex queueMutex;
        std::unordered_map<std::string, Texture2D> textureCache;
        std::unordered_map<std::string, int> textureRefCount;
        std::unordered_map<std::string, std::string> textureSceneMap; 
        std::unordered_set<std::string> activeSceneTextures;
        std::unordered_set<std::string> loadingTextures;
    
        std::queue<std::tuple<std::string, std::string, std::string>> imageQueue; 
        std::queue<std::tuple<std::string, Image, std::string>> pendingImages; 
    
        std::atomic<bool> running;
        std::thread workerThread;
    
        std::string currentScene;
        int CACHE_EXPIRATION_DAYS = 7;
        const std::string CACHE_DIR = "cache";
        std::unordered_map<std::string, Font> fontCache; 
    };
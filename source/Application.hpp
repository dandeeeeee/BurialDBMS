#pragma once
#include "raylib.h"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include "Home.hpp"
#include "Settings.hpp"
#include "Login.hpp"
#include <memory>
#include <map>
#include <iostream>


class Application
{
private:
    
    Camera camera;
    RenderTexture2D target;
    Shader shader;


public:

    // * Singleton
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;


    static Application& getInstance()
    {
        static Application instance;
        return instance;
    };


    ~Application()
    {
        CloseWindow();
    };


    void run() {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        constexpr int screenWidth = 800;
        constexpr int screenHeight = 450;
        InitWindow(screenWidth, screenHeight, "Raylib Template");
        SetTargetFPS(60);
    
        std::string textureUrl = "http://127.0.0.1:8000/images/bg.jpg";
        std::string textureFilename = "bg.png";
        std::string textureScene = "Login";
        ResourceManager::GetInstance().GetTexture(textureUrl, textureFilename, textureScene);
    
        while (!WindowShouldClose()) {
            // Process pending textures in the main thread
            ResourceManager::GetInstance().ProcessPendingImages();
    
            // Re-fetch the texture from the ResourceManager to check if it has been loaded
            Texture2D texture = ResourceManager::GetInstance().GetTexture(textureUrl, textureFilename, textureScene);
            BeginDrawing();
            ClearBackground(RAYWHITE);
    
            // Draw the texture if loaded
            if (texture.id != 0) {
                DrawTexturePro(texture, {0.0f, 0.0f, (float)texture.width, (float)texture.height},
                            {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()}, {0, 0}, 0.0f, WHITE);
                DrawRectangleGradientH(300, 0, GetScreenWidth(), GetScreenHeight(),  DARKBLUE, BLANK);
            }
            else {
                DrawText("Loading...", 10, 10, 20, RED);
            }
    
            EndDrawing();
        }
    
        // Unload the texture when done (optional, as ResourceManager will clean up)
    }

    
private:

    Application() 
    {
        currentScene = "Login";

        sceneFactory = // ? This is a factory pattern
        {
            {"Home", std::make_shared<Home>()},
            {"Settings", std::make_shared<Settings>()},
            {"Login", std::make_shared<Login>()}
        };
    };

    std::map<std::string, std::shared_ptr<Scene>> sceneFactory;
    std::string currentScene;

    // TODO: Implement a Scene class
    // std::array<std::shared_ptr<Scene>, 4> Scenes;
    // std::shared_ptr<Scene> currentScene;
};
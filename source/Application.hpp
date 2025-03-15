#pragma once
#include "raylib.h"
#include "raymath.h"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include "Home.hpp"
#include "Settings.hpp"
#include "Login.hpp"
#include <memory>
#include <map>
#include <iostream>

#define MIN(a,b) (((a)<(b))?(a):(b))


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


    void run() 
    {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        // 3840 × 2160
        constexpr int screenWidth = 1920;
        constexpr int screenHeight = 1080;
        InitWindow(1280, 720, "BurialDBMS");
        SetTargetFPS(60);

        target = LoadRenderTexture(screenWidth, screenHeight);
        SetTextureFilter(target.texture, TEXTURE_FILTER_TRILINEAR);

        ResourceManager::GetInstance().LoadFonts();
    
        while (!WindowShouldClose()) {
            // ? Process pending textures in the main thread
            ResourceManager::GetInstance().ProcessPendingImages();

            // ? Setup the render target
            float scale = MIN((float)GetScreenWidth()/screenWidth, (float)GetScreenHeight()/screenHeight);
            Vector2 mouse = GetMousePosition();
            Vector2 virtualMouse = { 0 };
            virtualMouse.x = (mouse.x - ((float)GetScreenWidth() - ((float)screenWidth*scale))*0.5f)/scale;
            virtualMouse.y = (mouse.y - ((float)GetScreenHeight() - ((float)screenHeight*scale))*0.5f)/scale;
            virtualMouse = Vector2Clamp(virtualMouse, Vector2Zero(), Vector2{ (float)screenWidth, (float)screenHeight });

            SetMouseOffset(static_cast<int>(-((float)GetScreenWidth() - ((float)screenWidth*scale))*0.5f),
                        static_cast<int>(-((float)GetScreenHeight() - ((float)screenHeight*scale))*0.5f));
            SetMouseScale(1/scale, 1/scale);


            // ? capture screen
            BeginTextureMode(target);

            ClearBackground(RAYWHITE);

                if (sceneFactory.find(currentScene) != sceneFactory.end())
                {
                    std::string nextScene = sceneFactory[currentScene]->update();
                    if (sceneFactory.find(nextScene) != sceneFactory.end())
                    {
                        if (nextScene != currentScene)
                        {
                            sceneFactory[currentScene]->onExit();
                            sceneFactory[nextScene]->onSwitch();
                        }
                        currentScene = nextScene;
                    }
                    else std::cout << "SCENE NOT FOUNDDDDDD" << std::endl; // TODO: Implement a logger
                    sceneFactory[currentScene]->draw();
                }  

            EndTextureMode();

            BeginDrawing();
                
                ClearBackground(RAYWHITE);
                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(SKYBLUE, 0.8f));


                // ? Draw the render target
                DrawTexturePro(target.texture, Rectangle{ 0.0f, 0.0f, (float)target.texture.width, (float)-target.texture.height }, 
                        Rectangle{((float)GetScreenWidth() - ((float)screenWidth*scale))*0.5f,
                                    ((float)GetScreenHeight() - ((float)screenHeight*scale))*0.5f,
                                    (float)screenWidth*scale, (float)screenHeight*scale},
                                    Vector2Zero(), 0.0f, WHITE);
    
            EndDrawing();
        }
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

        sceneFactory[currentScene]->onSwitch();
    };

    std::map<std::string, std::shared_ptr<Scene>> sceneFactory;
    std::string currentScene;

    // TODO: Implement a Scene class
    // std::array<std::shared_ptr<Scene>, 4> Scenes;
    // std::shared_ptr<Scene> currentScene;
};
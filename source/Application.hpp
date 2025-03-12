#pragma once
#include "raylib.h"
#include "Scene.hpp"
#include "Home.hpp"
#include "Settings.hpp"
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


    void run()
    {
        constexpr int screenWidth = 800;
        constexpr int screenHeight = 450;
        InitWindow(screenWidth, screenHeight, "Raylib Template");
        SetTargetFPS(60);

        while (!WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            if (sceneFactory.find(currentScene) != sceneFactory.end())
            {
                std::string nextScene = sceneFactory[currentScene]->update();
                if (sceneFactory.find(nextScene) != sceneFactory.end()) currentScene = nextScene;
                else std::cout << "SCENE NOT FOUNDDDDDD" << std::endl; // TODO: Implement a logger
                sceneFactory[currentScene]->draw();
            }  

            EndDrawing();
        }
    };

    
private:

    Application() 
    {
        currentScene = "Home";

        sceneFactory = // ? This is a factory pattern
        {
            {"Home", std::make_shared<Home>()},
            {"Settings", std::make_shared<Settings>()}
        };
    };

    std::map<std::string, std::shared_ptr<Scene>> sceneFactory;
    std::string currentScene;

    // TODO: Implement a Scene class
    // std::array<std::shared_ptr<Scene>, 4> Scenes;
    // std::shared_ptr<Scene> currentScene;
};
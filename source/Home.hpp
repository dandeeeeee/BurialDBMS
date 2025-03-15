#pragma once
#include "Scene.hpp"
#include "Utils.hpp"


class Home : public Scene
{
public:

    Home() = default;

    void onSwitch() override
    {
        // ? Load the home scene
    };

    void onExit() override
    {
        // ? Unload the home scene
    };

    std::string update() override
    {
        if (IsKeyPressed(KEY_ENTER))
        {   
            return "Login";
        }

        return "Home";
    };

    void draw() override
    {
        utils::DrawTexture("bg.png", "Home", {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()});
    };

    ~Home() = default;

private:

    std::string url;
};
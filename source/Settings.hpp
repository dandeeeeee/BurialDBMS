#pragma once
#include "Scene.hpp"


class Settings : public Scene
{

public:

    Settings() = default;

    void onSwitch() override
    {
        // ? Load the settings scene
    };

    void onExit() override
    {
        // ? Unload the settings scene
    };

    std::string update() override
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            return "Home";
        }

        return "Settings";
    };

    void draw() override
    {
        DrawRectangle(0, 0, 800, 450, BLUE);
    };

    ~Settings() = default;

private:

    std::string url;
};
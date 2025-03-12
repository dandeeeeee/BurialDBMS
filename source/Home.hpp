#pragma once
#include "Scene.hpp"


class Home : public Scene
{
public:

    Home() = default;

    std::string update() override
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            return "Settings";
        }

        return "Home";
    };

    void draw() override
    {
        DrawRectangle(0, 0, 800, 450, MAROON);
    };

    ~Home() = default;

private:

    std::string url;
};
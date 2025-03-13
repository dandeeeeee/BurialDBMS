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
        DrawRectangleGradientH(0, 0, 800, 450,  DARKBLUE, BLANK);
    };

    ~Home() = default;

private:

    std::string url;
};
#pragma once


#include "Scene.hpp"
#include "Utils.hpp"


class Login : public Scene
{

public:

    Login() = default;

    std::string update() override
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            return "Home";
        }

        return "Login";
    };

    void draw() override
    {
        utils::DrawTexture("bg.jpg", "Login", {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()});
    };

    ~Login() = default;


private:

    std::string username;
    std::string password;

    std::string url;

};
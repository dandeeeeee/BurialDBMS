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
        utils::DrawTexture("LoginBG.png", "Login", {0, 0, APP_WIDTH, APP_HEIGHT});
    };

    ~Login() = default;


private:

    std::string username;
    std::string password;

    std::string url;

};
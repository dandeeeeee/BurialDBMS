#pragma once


#include "Scene.hpp"


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
        DrawRectangle(0, 0, 800, 450, YELLOW);
    };

    ~Login() = default;


private:

    std::string username;
    std::string password;

    std::string url;

};
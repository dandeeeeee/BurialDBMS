#pragma once
#include "Scene.hpp"
#include "Utils.hpp"
#include "Styles.hpp"
#include <vector>


enum LoginState
{
    LOGIN = 0,
    SIGN_UP
};


class Login : public Scene
{

public:

    Login() = default;

    void onSwitch() override
    {
        std::cout << "Switched to Login" << std::endl;
        buttons.push_back(Button(Rectangle{150, 570, 500, 100}, Fade(SKYBLUE, 0.8f), 0.25, "LOGIN", 50));
        buttons.push_back(Button(Rectangle{150, 700, 500, 100}, Fade(BLUE, 0.8f), 0.25, "SIGN UP", 50));
        login = false;
        signup = false;
    };

    void onExit() override
    {
        std::cout << "Exited Login" << std::endl;
        buttons.clear();
    };

    std::string update() override
    {
        return "Login";
    };

    void draw() override
    {
        if (utils::DrawTexture("LoginBG.png", "Login", {0, 0, APP_WIDTH, APP_HEIGHT}))
        {
            if (!login && !signup)
            {
                for (auto& button : buttons)
                {
                    button.drawTextCentered();
                }
            }
            else if (login)
            {
                performLogin();
            }
            else if (signup)
            {
                performSignUp();
            }
        }

        if (buttons[LOGIN].isClicked()) login = true;
        else if (buttons[SIGN_UP].isClicked()) signup = true;
    };

    ~Login() = default;


private:

    void performLogin() // ? Send login request
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            login = false;
        }
    }

    void performSignUp() // ? Send signup request
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            signup = false;
        }
    }

    std::string username;
    std::string password;
    std::vector<Button> buttons;
    std::string url;

    bool login;
    bool signup;
};
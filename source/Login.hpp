#pragma once


#include "Scene.hpp"
#include "Utils.hpp"
#include "Styles.hpp"
#include <vector>


class Login : public Scene
{

public:

    Login() 
    {
        buttons.push_back(Button(Rectangle{150, 570, 500, 100}, Fade(SKYBLUE, 0.8f), 0.25, "LOGIN", 50));
        buttons.push_back(Button(Rectangle{150, 700, 500, 100}, Fade(BLUE, 0.8f), 0.25, "REGISTER", 50));
    };

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

        for (auto& button : buttons)
        {
            button.drawTextCentered();
        }
    };

    ~Login() = default;


private:

    std::string username;
    std::string password;
    std::vector<Button> buttons;
    std::string url;

};
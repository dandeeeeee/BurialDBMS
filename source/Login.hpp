#pragma once
#include "Scene.hpp"
#include "Utils.hpp"
#include "Styles.hpp"
#include <vector>

enum LoginState
{
    LOGIN = 0,
    SIGN_UP,
    BACK
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
        currentState = LOGIN;
        login = false;
        signup = false;

        backButton = Button(Rectangle{150, 570, 500, 100}, Fade(GRAY, 0.8f), 0.25, "BACK", 50);
        backButton.setOpacity(0.0f);
        backButton.fadeOut(); 

        for (auto& button : buttons) button.setOpacity(0.8f);
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
            for (auto& button : buttons) {
                button.updateFade(0.8f);
            }
            backButton.updateFade(0.8f);

            if (currentState == LOGIN || currentState == SIGN_UP)
            {
                for (auto& button : buttons) {
                    if (button.isVisibleAndInteractable()) {
                        button.drawTextCentered();
                    }
                }

                if (buttons[LOGIN].isClicked()) {
                    buttons[LOGIN].fadeOut();
                    buttons[SIGN_UP].fadeOut();
                    backButton.fadeIn(); 
                    currentState = BACK;
                }
                else if (buttons[SIGN_UP].isClicked()) {
                    buttons[LOGIN].fadeOut();
                    buttons[SIGN_UP].fadeOut();
                    backButton.fadeIn();
                    currentState = BACK;
                }
            }
            else if (currentState == BACK)
            {
                if (backButton.isVisibleAndInteractable()) {
                    backButton.drawTextCentered();
                }

                if (backButton.isClicked()) {
                    backButton.fadeOut(); 
                    buttons[LOGIN].fadeIn();
                    buttons[SIGN_UP].fadeIn();
                    currentState = LOGIN;
                }
            }
        }
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
    Button backButton;

    bool login;
    bool signup;

    LoginState currentState;
};
#pragma once
#include "Scene.hpp"
#include "Utils.hpp"
#include "Styles.hpp"
#include "Textbox.hpp"
#include <vector>

#include <iostream>

enum LoginState
{
    LOGIN = 0,
    SIGN_UP,
    BACK
};

class Login : public Scene
{
public:

    Login() 
        : backButton(Rectangle{150, 570, 500, 100}, Fade(GRAY, 0.8f), 0.25, "BACK", 50),
        usernameTextBox(Rectangle{150, 400, 500, 50}, 20, BLACK, DARKGRAY),
        passwordTextBox(Rectangle{150, 470, 500, 50}, 20, BLACK, DARKGRAY)
        {}
    
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

        usernameTextBox = TextBox(Rectangle{150, 400, 500, 50}, 20, BLACK, DARKGRAY);
        passwordTextBox = TextBox(Rectangle{150, 470, 500, 50}, 20, BLACK, DARKGRAY);

        for (auto& button : buttons) button.setOpacity(0.8f);
    };

    void onExit() override
    {
        std::cout << "Exited Login" << std::endl;
        buttons.clear();
        backButton = Button();
        usernameTextBox = TextBox();
        passwordTextBox = TextBox();
    };

    std::string update() override
    {
        usernameTextBox.update();
        passwordTextBox.update();
        // std::cout << "Username: " << usernameTextBox.getText() << std::endl;
        // std::cout << "Password: " << passwordTextBox.getText() << std::endl;
        return "Login";
    };

    void draw() override
    {
        if (utils::DrawTexture("LoginBG.png", "Login", {0, 0, APP_WIDTH, APP_HEIGHT}))
        {
            usernameTextBox.draw();
            passwordTextBox.draw();

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

    TextBox usernameTextBox;
    TextBox passwordTextBox;

    bool login;
    bool signup;

    LoginState currentState;
};
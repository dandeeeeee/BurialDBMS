#pragma once
#include "GUI.hpp"
#include "raylib.h"
#include <string>
#include <algorithm>

#define NOMINMAX

#define BACKSPACE_DELETION_DELAY 0.15f
#define ARROW_KEY_DELAY BACKSPACE_DELETION_DELAY
#define STANDARD_LINE_SPACING 2.0f


// TODO: ADD COPY AND PASTE SUPPORT?

class TextBox : public GUI
{
private:
    Rectangle bounds;
    std::string text;
    int fontSize;
    Color textColor;
    Color cursorColor;
    bool isFocused;
    int cursorPosition;
    float cursorBlinkTime;
    bool cursorVisible;
    float textOffsetX;
    float backspaceTimer;
    float leftArrowTimer;
    float rightArrowTimer;

public:
    TextBox() = default;

    TextBox(Rectangle bounds, int fontSize, Color textColor, Color cursorColor)
        : bounds(bounds), fontSize(fontSize), textColor(textColor), cursorColor(cursorColor),
          isFocused(false), cursorPosition(0), cursorBlinkTime(0.0f), cursorVisible(true), textOffsetX(0.0f),
          backspaceTimer(0.0f), leftArrowTimer(0.0f), rightArrowTimer(0.0f)
    {
        text = "";
    }

    void update() {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isFocused = CheckCollisionPointRec(GetMousePosition(), bounds);
        }

        if (isFocused) {
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (cursorVisible) cursorBlinkTime = 0.0f;
                else cursorVisible = true;
                if (cursorPosition > 0) {
                    text.erase(cursorPosition - 1, 1);
                    cursorPosition--;
                    adjustTextOffset();
                }
                backspaceTimer = 0.0f;
            } else if (IsKeyDown(KEY_BACKSPACE)) {
                if (cursorVisible) cursorBlinkTime = 0.0f;
                else cursorVisible = true;
                backspaceTimer += GetFrameTime();
                if (backspaceTimer >= BACKSPACE_DELETION_DELAY) {
                    if (cursorPosition > 0) {
                        text.erase(cursorPosition - 1, 1);
                        cursorPosition--;
                        adjustTextOffset();
                    }
                    backspaceTimer = 0.1f;
                }
            }

            if (IsKeyPressed(KEY_DELETE) && cursorPosition < static_cast<int>(text.length())) {
                if (cursorVisible) cursorBlinkTime = 0.0f;
                else cursorVisible = true;
                text.erase(cursorPosition, 1);
                adjustTextOffset();
            }

            if (IsKeyPressed(KEY_LEFT)) {
                if (cursorVisible) cursorBlinkTime = 0.0f;
                else cursorVisible = true;
                if (cursorPosition > 0) {
                    cursorPosition--;
                    adjustTextOffset();
                }
                leftArrowTimer = 0.0f;
            } else if (IsKeyDown(KEY_LEFT)) {
                if (cursorVisible) cursorBlinkTime = 0.0f;
                else cursorVisible = true;
                leftArrowTimer += GetFrameTime();
                if (leftArrowTimer >= ARROW_KEY_DELAY) {
                    if (cursorPosition > 0) {
                        cursorPosition--;
                        adjustTextOffset();
                    }
                    leftArrowTimer = 0.1f;
                }
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                if (cursorVisible) cursorBlinkTime = 0.0f;
                else cursorVisible = true;
                if (cursorPosition < static_cast<int>(text.length())) {
                    cursorPosition++;
                    adjustTextOffset();
                }
                rightArrowTimer = 0.0f;
            } else if (IsKeyDown(KEY_RIGHT)) {
                if (cursorVisible) cursorBlinkTime = 0.0f;
                else cursorVisible = true;
                rightArrowTimer += GetFrameTime();
                if (rightArrowTimer >= ARROW_KEY_DELAY) {
                    if (cursorPosition < static_cast<int>(text.length())) {
                        cursorPosition++;
                        adjustTextOffset();
                    }
                    rightArrowTimer = 0.1f;
                }
            }

            int key = GetCharPressed();
            while (key > 0) {
                if (cursorVisible) cursorBlinkTime = 0.0f;
                else cursorVisible = true;
                if (key >= 32 && key <= 126) {
                    text.insert(cursorPosition, 1, static_cast<char>(key));
                    cursorPosition++;
                    adjustTextOffset();
                }
                key = GetCharPressed();
            }

            cursorBlinkTime += GetFrameTime();
            if (cursorBlinkTime >= 0.5f) {
                cursorBlinkTime = 0.0f;
                cursorVisible = !cursorVisible;
            }
        }
    }

    void draw() {
        DrawRectangleRec(bounds, Fade(LIGHTGRAY, opacity));

        BeginScissorMode(static_cast<int>(bounds.x), static_cast<int>(bounds.y),
                         static_cast<int>(bounds.width), static_cast<int>(bounds.height));

        Vector2 textPosition = { bounds.x + 5 - textOffsetX, bounds.y + (bounds.height - fontSize) / 2 };
        DrawTextEx(GetFontDefault(), text.c_str(), textPosition, static_cast<float>(fontSize), STANDARD_LINE_SPACING, textColor);

        if (isFocused && cursorVisible) {
            Vector2 cursorPositionVec = MeasureTextEx(GetFontDefault(), text.substr(0, cursorPosition).c_str(), static_cast<float>(fontSize), STANDARD_LINE_SPACING);
            DrawLine(static_cast<int>(bounds.x + 5 + cursorPositionVec.x - textOffsetX), static_cast<int>(bounds.y + 5),
                     static_cast<int>(bounds.x + 5 + cursorPositionVec.x - textOffsetX), static_cast<int>(bounds.y + bounds.height - 5),
                     cursorColor);
        }

        EndScissorMode();
    }

    std::string getText() const {
        return text;
    }

    void setText(const std::string& newText) {
        text = newText;
        cursorPosition = static_cast<int>(text.length());
        adjustTextOffset();
    }

    bool getIsFocused() const {
        return isFocused;
    }

    void setFocus(bool focus) {
        isFocused = focus;
        if (focus) {
            cursorVisible = true;
            cursorBlinkTime = 0.0f;
        }
    }

private:
    void adjustTextOffset() {
        float textWidth = MeasureTextEx(GetFontDefault(), text.c_str(), static_cast<float>(fontSize), STANDARD_LINE_SPACING).x;
        float cursorX = MeasureTextEx(GetFontDefault(), text.substr(0, cursorPosition).c_str(), static_cast<float>(fontSize), STANDARD_LINE_SPACING).x;

        if (cursorX - textOffsetX > bounds.width - 10) {
            textOffsetX = cursorX - (bounds.width - 10);
        } else if (cursorX - textOffsetX < 0) {
            textOffsetX = cursorX;
        }

        textOffsetX = (std::max)(0.0f, (std::min)(textOffsetX, textWidth - bounds.width + 10));
    }
};

#pragma once
#include "raylib.h"
#include "raymath.h"
#include "ResourceManager.hpp"
#include "Utils.hpp"
#include <string>

class Button
{
private:
    Rectangle rec;
    Color color;
    float roundness;
    std::string text;
    int fontSize;
    // animation
    bool clicked = false;
    float shrinkFactor = 0.0f;
    const float shrinkAmount = 10.0f;

public:
    Button(Rectangle rec, Color color, float roundness, std::string text, int fontSize) 
        : rec(rec), color(color), roundness(roundness), text(text), fontSize(fontSize) {}

    void drawTextCentered()
    {   
        Font font = ResourceManager::GetInstance().GetFont("LeagueSpartan-Bold.ttf");
        Vector2 textSize = MeasureTextEx(font, text.c_str(), static_cast<float>(fontSize), 5);
        Vector2 textPos = { rec.x + rec.width / 2 - textSize.x / 2, rec.y + rec.height / 2 - textSize.y / 2 };

        //? Determine if button is clicked
        bool isClicked = CheckCollisionPointRec(GetMousePosition(), rec) && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        if (isClicked) {
            clicked = true;
            shrinkFactor = shrinkAmount;
        } else {
            clicked = false;
            shrinkFactor = 0.0f;
        }

        //? Shrink button size
        float adjustedWidth = rec.width - shrinkFactor;
        float adjustedHeight = rec.height - shrinkFactor;
        Vector2 adjustedPos = { rec.x + shrinkFactor / 2, rec.y + shrinkFactor / 2 };

        //? Shrink text size proportionally
        int adjustedFontSize = fontSize - (clicked ? 5 : 0);
        Vector2 adjustedTextSize = MeasureTextEx(font, text.c_str(), static_cast<float>(adjustedFontSize), 5);
        Vector2 adjustedTextPos = { 
            adjustedPos.x + adjustedWidth / 2 - adjustedTextSize.x / 2, 
            adjustedPos.y + adjustedHeight / 2 - adjustedTextSize.y / 2 
        };

        //? Draw button
        DrawRectangleRounded(
            Rectangle{ adjustedPos.x, adjustedPos.y, adjustedWidth, adjustedHeight }, 
            roundness, 0, 
            isHovered() ? Fade(RAYWHITE, 0.8f) : color
        );

        //? Draw text
        utils::DrawText("LeagueSpartan-Bold.ttf", text, adjustedTextPos, adjustedFontSize, RAYWHITE);
    }

    bool isHovered() const
    {
        return CheckCollisionPointRec(GetMousePosition(), rec);
    }

    bool isClicked() const
    {
        return isHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    void setText(std::string newText)
    {
        text = newText;
    }
};

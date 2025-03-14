#pragma once
#include "raylib.h"
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

public:

    Button(Rectangle rec, Color color, float roundness, std::string text, int fontSize) : rec(rec), color(color), roundness(roundness), text(text), fontSize(fontSize) {};

    void drawTextCentered() const
    {   
        Vector2 textSize = MeasureTextEx(ResourceManager::GetInstance().GetFont("LeagueSpartan-Bold.ttf"), text.c_str(), static_cast<float>(fontSize), 5);
        Vector2 textPos = {rec.x + rec.width/2 - textSize.x/2, rec.y + rec.height/2 - textSize.y/2};

        DrawRectangleRounded(rec, roundness, 0, (isHovered() ? Fade(RAYWHITE, 0.8f) : color));
        utils::DrawText("LeagueSpartan-Bold.ttf", text, textPos, fontSize, RAYWHITE);
    };

    bool isHovered() const
    {
        return CheckCollisionPointRec(GetMousePosition(), rec);
    };

    bool isClicked() const
    {
        return isHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    };
    
    void setText(std::string text)
    {
        this->text = text;
    };
};
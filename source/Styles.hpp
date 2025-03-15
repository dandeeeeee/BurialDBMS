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
    
    // Click Animation
    bool held = false;  
    bool clicked = false;  
    float shrinkFactor = 0.0f;
    const float shrinkAmount = 10.0f;

    // Hover Animation
    float hoverWidth = 1.0f;  
    float hoverHeight = 1.0f;  
    float hoverRoundness = 1.0f; 
    Vector2 hoverPos;
    bool isHovering = false;
    const float hoverExpandSpeed = 2500.0f;
    const float lerpFactor = 30.0f;

public:
    Button(Rectangle rec, Color color, float roundness, std::string text, int fontSize) 
        : rec(rec), color(color), roundness(roundness), text(text), fontSize(fontSize)
    {
        hoverPos = { rec.x + rec.width / 2, rec.y + rec.height / 2 }; 
    }

    void drawTextCentered()
    {   
        Font font = ResourceManager::GetInstance().GetFont("LeagueSpartan-Bold.ttf");
        Vector2 textSize = MeasureTextEx(font, text.c_str(), static_cast<float>(fontSize), 5);
        Vector2 textPos = { rec.x + rec.width / 2 - textSize.x / 2, rec.y + rec.height / 2 - textSize.y / 2 };

        // Click Animation
        if (isHovered() && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            held = true;  
            shrinkFactor = shrinkAmount;
        } 
        else if (held && IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isHovered()) {
            clicked = true;
            shrinkFactor = 0.0f;
        } 
        else {
            held = false;
            shrinkFactor = 0.0f;
        }

        float adjustedWidth = rec.width - shrinkFactor;
        float adjustedHeight = rec.height - shrinkFactor;
        Vector2 adjustedPos = { rec.x + shrinkFactor / 2, rec.y + shrinkFactor / 2 };


        if (isHovered()) {
            isHovering = true;
            hoverWidth = Lerp(hoverWidth, rec.width, lerpFactor * GetFrameTime());
            hoverHeight = Lerp(hoverHeight, rec.height, lerpFactor * GetFrameTime());
            hoverRoundness = Lerp(hoverRoundness, rec.height / 2.0f, lerpFactor * GetFrameTime()); 

            if (hoverHeight >= rec.height - 1) {  
                hoverHeight = rec.height; 
                hoverWidth = Lerp(hoverWidth, rec.width, lerpFactor * GetFrameTime());
                if (hoverWidth >= rec.width - 1) 
                {
                    hoverWidth = rec.width; 
                    if (hoverRoundness >= roundness) hoverRoundness = roundness; 
                }
            }
        } 
        else {
            isHovering = false;
            hoverWidth = Lerp(hoverWidth, 1.0f, lerpFactor * GetFrameTime());
            hoverHeight = Lerp(hoverHeight, 1.0f, lerpFactor * GetFrameTime());
            hoverRoundness = Lerp(hoverRoundness, 1.0f, lerpFactor * GetFrameTime());
        }


        hoverPos.x = rec.x + rec.width / 2 - hoverWidth / 2;
        hoverPos.y = rec.y + rec.height / 2 - hoverHeight / 2;


        

  
        DrawRectangleRounded(
            Rectangle{ adjustedPos.x, adjustedPos.y, adjustedWidth, adjustedHeight }, 
            roundness, 0, 
            color
        );

        DrawRectangleRounded(
            Rectangle{ hoverPos.x, hoverPos.y, hoverWidth, hoverHeight },
            hoverRoundness, 0, Fade(RAYWHITE, 0.8f)
        );

  
        int adjustedFontSize = fontSize - (held ? 5 : 0);
        Vector2 adjustedTextSize = MeasureTextEx(font, text.c_str(), static_cast<float>(adjustedFontSize), 5);
        Vector2 adjustedTextPos = { 
            adjustedPos.x + adjustedWidth / 2 - adjustedTextSize.x / 2, 
            adjustedPos.y + adjustedHeight / 2 - adjustedTextSize.y / 2 
        };


        utils::DrawText("LeagueSpartan-Bold.ttf", text, adjustedTextPos, adjustedFontSize, isHovering ? color : RAYWHITE);
    }

    bool isHovered() const
    {
        return CheckCollisionPointRec(GetMousePosition(), rec);
    }

    bool isClicked()
    {
        if (clicked) {
            clicked = false;
            return true;
        }
        return false;
    }

    void setText(std::string newText)
    {
        text = newText;
    }
};

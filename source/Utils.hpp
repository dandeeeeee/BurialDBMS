#pragma once
#include "raylib.h"
#include "ResourceManager.hpp"
#include <string>

#define APP_WIDTH 1920
#define APP_HEIGHT 1080

namespace utils
{
    inline void DrawTexture(std::string filename, std::string scene, Rectangle rec) 
    {
        Texture2D texture = ResourceManager::GetInstance().GetTexture(filename, scene);
        if (texture.id != 0) {
            DrawTexturePro(texture,
                {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)},
                rec,
                {0, 0},
                0.0f,
                WHITE);
        }
        else 
        {
            DrawText("Loading...", (rec.x + rec.width / 2) - 20, (rec.y + rec.height / 2) - 20, 20, RED);
        }
    };

    inline void DrawText(std::string filename, std::string text, Vector2 pos, int fontSize, Color color)
    {
        Font font = ResourceManager::GetInstance().GetFont(filename);
        if (font.glyphs != nullptr && font.baseSize > 0) {
            DrawTextEx(font, text.c_str(), pos, fontSize, 0, color);
        } else {
            DrawTextEx(GetFontDefault(), text.c_str(), pos, fontSize, 0, color);
        }
    };
};
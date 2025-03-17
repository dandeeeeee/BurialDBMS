#pragma once
#include "raylib.h"
#include "ResourceManager.hpp"
#include <string>

#define APP_WIDTH 3840
#define APP_HEIGHT 2160

namespace utils
{
    inline bool DrawTexture(std::string filename, std::string scene, Rectangle rec) 
    {
        Texture2D texture = ResourceManager::GetInstance().GetTexture(filename, scene);
        if (texture.id != 0) {
            DrawTexturePro(texture,
                {0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)},
                rec,
                {0, 0},
                0.0f,
                WHITE);
            return true;
        }
        else 
        {
            DrawText("Loading...", static_cast<int>((rec.x + rec.width / 2) - 50), static_cast<int>((rec.y + rec.height / 2)) - 50, 50, DARKBLUE);
            return false;
        }
    };

    inline void DrawText(std::string filename, std::string text, Vector2 pos, int fontSize, Color color)
    {
        Font font = ResourceManager::GetInstance().GetFont(filename);
        if (font.glyphs != nullptr && font.baseSize > 0) {
            DrawTextEx(font, text.c_str(), pos, static_cast<float>(fontSize), 0, color);
        } else {
            DrawTextEx(GetFontDefault(), text.c_str(), pos, static_cast<float>(fontSize), 0, color);
        }
    };
};
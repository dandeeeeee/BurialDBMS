#pragma once
#include "raylib.h"
#include "ResourceManager.hpp"
#include <string>


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
};
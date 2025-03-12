#pragma once
#include "raylib.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>


/*
    * This class is the Resource Manager that manages resources such as textures, fonts, shaders, and sounds.
    ?   Features:
    ?   ✅ Lazy Loading – Resources are loaded only when needed.
    ?   ✅ Automatic Resource Cleanup – Unloads all resources when the program exits.
    ?   ✅ Preloading Support – Allows caching frequently used resources.
    ?   ✅ Thread-Safe Singleton – Ensures only one instance exists.
*/

class ResourceManager
{
public:

    static ResourceManager& getInstance()
    {
        static ResourceManager instance;
        return instance;
    }


    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    
    std::shared_ptr<Texture2D> loadTexture(const std::string& name, const std::string& path)
    {
        if (textures.find(name) == textures.end())
        {
            auto texture = std::make_shared<Texture2D>(LoadTexture(path.c_str()));
            if (texture->id == 0)
            {
                std::cerr << "Failed to load texture: " << path << std::endl;
                return nullptr;
            }
            textures[name] = texture;
        }
        return textures[name];
    }

    
    std::shared_ptr<Texture2D> getTexture(const std::string& name)
    {
        if (textures.find(name) != textures.end())
            return textures[name];
        std::cerr << "Texture not found: " << name << std::endl;
        return nullptr;
    }

    
    std::shared_ptr<Font> loadFont(const std::string& name, const std::string& path, int size = 20)
    {
        if (fonts.find(name) == fonts.end())
        {
            auto font = std::make_shared<Font>(LoadFontEx(path.c_str(), size, nullptr, 0));
            if (font->texture.id == 0)
            {
                std::cerr << "Failed to load font: " << path << std::endl;
                return nullptr;
            }
            fonts[name] = font;
        }
        return fonts[name];
    }


    std::shared_ptr<Font> getFont(const std::string& name)
    {
        if (fonts.find(name) != fonts.end())
            return fonts[name];
        std::cerr << "Font not found: " << name << std::endl;
        return nullptr;
    }

    
    std::shared_ptr<Shader> loadShader(const std::string& name, const std::string& vsPath, const std::string& fsPath)
    {
        if (shaders.find(name) == shaders.end())
        {
            auto shader = std::make_shared<Shader>(LoadShader(vsPath.c_str(), fsPath.c_str()));
            if (shader->id == 0)
            {
                std::cerr << "Failed to load shader: " << vsPath << " / " << fsPath << std::endl;
                return nullptr;
            }
            shaders[name] = shader;
        }
        return shaders[name];
    }

  
    std::shared_ptr<Shader> getShader(const std::string& name)
    {
        if (shaders.find(name) != shaders.end())
            return shaders[name];
        std::cerr << "Shader not found: " << name << std::endl;
        return nullptr;
    }

   
    std::shared_ptr<Sound> loadSound(const std::string& name, const std::string& path)
    {
        if (sounds.find(name) == sounds.end())
        {
            auto sound = std::make_shared<Sound>(LoadSound(path.c_str()));
            if (sound->frameCount == 0)
            {
                std::cerr << "Failed to load sound: " << path << std::endl;
                return nullptr;
            }
            sounds[name] = sound;
        }
        return sounds[name];
    }

   
    std::shared_ptr<Sound> getSound(const std::string& name)
    {
        if (sounds.find(name) != sounds.end())
            return sounds[name];
        std::cerr << "Sound not found: " << name << std::endl;
        return nullptr;
    }

   
    void unloadAll()
    {
        for (auto& [key, texture] : textures) UnloadTexture(*texture);
        for (auto& [key, font] : fonts) UnloadFont(*font);
        for (auto& [key, shader] : shaders) UnloadShader(*shader);
        for (auto& [key, sound] : sounds) UnloadSound(*sound);

        textures.clear();
        fonts.clear();
        shaders.clear();
        sounds.clear();
    }

    ~ResourceManager()
    {
        unloadAll();
        std::cout << "ResourceManager cleaned up.\n";
    }

private:
  
    ResourceManager() = default;

    std::unordered_map<std::string, std::shared_ptr<Texture2D>> textures;
    std::unordered_map<std::string, std::shared_ptr<Font>> fonts;
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    std::unordered_map<std::string, std::shared_ptr<Sound>> sounds;
};

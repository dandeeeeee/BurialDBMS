#pragma once

#include "raylib.h"

// ? This is a hack to fix an issue with the Windows API and Raylib
 
#if defined(_WIN32)           
    #define NOGDI            
    #define NOUSER           
#endif

#include <Windows.h> 

#if defined(_WIN32)           
    #undef far
#endif

#include "cpr/cpr.h"
#include "json.hpp"

using json = nlohmann::json;
#include <string>


class Scene
{
public:

    Scene() = default;
    virtual std::string update() = 0; // ? returns the name of the current or next scene
    virtual void draw() = 0;
    virtual ~Scene() = default;

private:

    // TODO: Json data
};
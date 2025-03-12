#include "raylib.h"
 
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
#include <iostream>

using json = nlohmann::json;

int main() {
  
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Cemetery Management - Test");
    
   
    std::cout << "Fetching cemetery layout from FastAPI server...\n";
    cpr::Response r = cpr::Get(cpr::Url{"http://127.0.0.1:8000/get_layout"});
    
    if (r.status_code == 200) {
        std::cout << "Server response: " << r.text << "\n";
        
       
        json layoutData = json::parse(r.text);
        
        
        if (!layoutData.empty()) {
            float x = layoutData[1]["x"];
            float y = layoutData[1]["y"];
            float width = layoutData[1]["width"];
            float height = layoutData[1]["height"];
            std::string status = layoutData[1]["status"];
            
            std::cout << "First burial plot: (" << x << ", " << y << "), size: " 
                      << width << "x" << height << ", status: " << status << "\n";
            
     
            while (!WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                
               
                DrawRectangle(x, y, width, height, (status == "occupied") ? RED : GREEN);
                DrawText("Cemetery Layout Test", 10, 10, 20, BLACK);
                
                EndDrawing();
            }
        } else {
            std::cout << "No burial plot data received!\n";
        }
    } else {
        std::cerr << "Failed to fetch data. HTTP Status: " << r.status_code << "\n";
    }

   
    CloseWindow();
    return 0;
}

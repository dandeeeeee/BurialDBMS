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
#include <vector>

using json = nlohmann::json;
using namespace std;

struct GravePlot {

    int ID;
    string status;
    string occupantName;
    // 
    int positionX;
    int positionY;
    int width;
    int height;

    GravePlot() {
        this->ID = NULL;
        this->status = "available";
        this->occupantName = "";
        this->positionX = GetMousePosition().x;
        this->positionY = GetMousePosition().y;
        this->width = 50;
        this->height = 50;
    }

    void UpdateMetadata(int ID, string status, string occupantName, Rectangle rect) {
        this->ID = ID;
        this->status = status;
        if (status == "occupied" || status == "reserved") {
            this->occupantName = occupantName;
        }
        this->positionX = rect.x;
        this->positionY = rect.y;
        this->width = rect.width;
        this->height = rect.height;
    }

    json convertToJson() {
        json j;
        j["ID"] = ID;
        j["status"] = status;
        j["occupantName"] = occupantName;
        j["positionX"] = positionX;
        j["positionY"] = positionY;
        j["width"] = width;
        j["height"] = height;

        if (!occupantName.empty()) {
            j["occupantName"] = occupantName;
        }

        return j;
    }

};

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "BurialDBMS");
    SetTargetFPS(60);

    // Load the grave plot data from the server
    cpr::Response r = cpr::Get(cpr::Url{"http://127.0.0.1:8000/graveplots"});
    json gravePlotData = json::parse(r.text);

    vector<GravePlot> gravePlots;
    if (!gravePlotData.is_null()) {
        for (auto& plot : gravePlotData) {
            GravePlot gp;
            gp.UpdateMetadata(plot["ID"], plot["status"], plot["occupantName"], Rectangle{plot["positionX"], plot["positionY"], plot["width"], plot["height"]});
            gravePlots.push_back(gp);
        }
    }
    else cout << "Failed to load grave plot data from the server." << endl;

    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto& plot : gravePlots) {
            if (plot.status == "available") {
                DrawRectangle(plot.positionX, plot.positionY, plot.width, plot.height, GREEN);
            }
            else if (plot.status == "occupied") {
                DrawRectangle(plot.positionX, plot.positionY, plot.width, plot.height, RED);
            }
            else if (plot.status == "reserved") {
                DrawRectangle(plot.positionX, plot.positionY, plot.width, plot.height, BLUE);
            }
        }
        
        EndDrawing();
    }

    CloseWindow();
}
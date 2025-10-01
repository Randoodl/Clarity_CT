#include "../include/ToolContainer.h"

//RGB Colour picker tool using raylib

int main(int argc, char* argv[])
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);    
    SetTraceLogLevel(7); //toggle Raylib output

    //The path of the running executable
    char* BinPath {argv[0]};
    
    //Starting with a small window and updating the minimum size at runtime
    InitWindow(100, 100, "Clarity Colour Tool"); 
    SetTargetFPS(60);

    ToolContainer ToolInstance = ToolContainer(BinPath);
    ToolInstance.BoxFont = LoadFont("./assets/VCR_OSD_MONO.ttf"); //Font VCR_OSD_MONO, courtesy of Riciery Santos Leal
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(ToolInstance.ColourCollection.BackgroundColour);
            ToolInstance.DrawElements();
        EndDrawing();

        ToolInstance.MouseClickHandler();
    }
    UnloadFont(ToolInstance.BoxFont);
    CloseWindow();

    return 0;
}
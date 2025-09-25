#include "../include/ToolContainer.h"

//RGB Colour picker tool using raylib

int main(int argc, char* argv[])
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);    
    SetTraceLogLevel(7); //toggle Raylib output

    //The path of the running executable
    char* BinPath {argv[0]};
    
    InitWindow(100, 100, "Clarity Colour Tool"); //Starting with a small window and updating the minimum size at runtime
    SetTargetFPS(60); //Perhaps not super relevant

    ToolContainer ToolInstance = ToolContainer(BinPath);
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(ToolInstance.ColourCollection.BackgroundColour);
            ToolInstance.DrawElements();
        EndDrawing();

        ToolInstance.MouseClickHandler();
    }
    CloseWindow();

    return 0;
}
#include "../include/ToolContainer.h"

//RGB Colour picker tool using raylib

int main(int argc, char* argv[])
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);    

    //The path of the running executable
    char* BinPath {argv[0]};
    
    InitWindow(910, 390, "Clarity CT");
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
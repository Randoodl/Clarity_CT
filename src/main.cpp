#include "../include/ToolContainer.h"

//RGB Colour picker tool using raylib

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);    
    
    InitWindow(910, 390, "Clarity CT");
    SetTargetFPS(60); //Perhaps not super relevant

    ToolContainer ToolInstance;
    
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
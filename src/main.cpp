#include "../include/ToolContainer.h"

//RGB Colour picker tool using raylib

int main()
{
    Color BackGroundColour {51, 66, 89, 255};

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);    
    
    InitWindow(1530, 750, "Clarity CT");
    SetTargetFPS(60); //Perhaps not super relevant

    ToolContainer ToolInstance;
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BackGroundColour);
            ToolInstance.DrawElements();
        EndDrawing();

        ToolInstance.MouseClickHandler();
    }
    CloseWindow();

    return 0;
}
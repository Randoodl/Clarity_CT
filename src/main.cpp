#include "../include/ToolContainer.h"

//RGB Colour picker tool using raylib

int main()
{
    Color BackGroundColour {51, 66, 89, 255};
    InitWindow(1530, 750, "RGB Colour Tool");
    SetTargetFPS(30); //Perhaps not super relevant

    ToolContainer ToolInstance; //Create all widgets (Colour Picker)
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BackGroundColour);
            ToolInstance.DrawElements();
      
        EndDrawing();

        if(IsMouseButtonDown(0))
        {
            ToolInstance.LeftMouseClickHandler();
        }
    }

    return 0;
}
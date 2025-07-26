#include "../include/ToolContainer.h"

//RGB Colour picker tool using raylib

int main()
{
    Color BackGroundColour {51, 66, 89, 255};
    InitWindow(1530, 750, "RGB Colour Tool");
    SetTargetFPS(30); //Perhaps not super relevant

    
    ToolContainer ToolInstance;


    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BackGroundColour);
            ToolInstance.RGBDial.DrawRGBDial(); 
            ToolInstance.RGBSquare.DrawGradientSquare();

      
        EndDrawing();

        if(IsMouseButtonDown(0))
        {
            ToolInstance.LeftMouseClickHandler();            
        }
    }

    return 0;
}
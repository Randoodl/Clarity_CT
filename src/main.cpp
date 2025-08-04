#include "../include/ToolContainer.h"

//RGB Colour picker tool using raylib

int main()
{
    Color BackGroundColour {51, 66, 89, 255};

    //SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);     //Perhaps we need to resize the window only after updating a Frame
    SetConfigFlags(FLAG_MSAA_4X_HINT); 
   
    InitWindow(1530, 750, "Clarity CT");
    //SetTargetFPS(30); //Perhaps not super relevant

    ToolContainer ToolInstance; //Create all widgets (Colour Picker)
    
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
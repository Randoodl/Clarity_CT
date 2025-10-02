/* 
 * Copyright (c) 2025, Dylan Ooijevaar
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../include/ToolContainer.h"

//RGB Colour picker tool using raylib v5.6 (www.raylib.com)

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
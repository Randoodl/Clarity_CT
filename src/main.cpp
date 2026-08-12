/* 
 * Copyright (c) 2026, Dylan Ooijevaar
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

#include <sys/stat.h>
#include <sys/types.h>

#include "../include/ToolContainer.h"
#include "../include/EmbeddedFont.h"

//RGB Colour picker tool using raylib v5.6 (www.raylib.com)

int main(int argc, char* argv[])
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);   
    SetTraceLogLevel(7); //toggle Raylib output

    //Set up config directory for Windows/Linux
    #ifdef __WIN32
        std::string ConfigPath {getenv("APPDATA") + (std::string)"\\ClarityCT"};
        mkdir(ConfigPath.c_str())
    #elif __linux__
        std::string ConfigPath {getenv("HOME") + (std::string)"/.config/ClarityCT"};
        mkdir(ConfigPath.c_str(), 0777);
    #endif    

    //Starting with a small window and updating the minimum size at runtime
    InitWindow(100, 100, "Clarity Colour Tool"); 
    SetTargetFPS(60);

    ToolContainer ToolInstance = ToolContainer(ConfigPath);
    ToolInstance.BoxFont = LoadFont_EmbeddedFont();   //Font: ari-w9500-display.ttf courtesy of Catterio Sylt
    
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
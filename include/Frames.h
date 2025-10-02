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

#pragma once

#include <raylib.h>
#include <vector> 


class Frames
{
    //This class is meant to create the frames to move and scale elements
    
    public:
        Frames();

        //Positional attributes for the whole Frame
        Rectangle FrameArea;  //XY absolute in relation to MainWindowXY
        int EdgeButtonSize;
        Rectangle MoveButton;
        Rectangle ScaleButton;

        //Frame states
        bool ActiveFrame;
        bool IsDragging;
        bool IsScaling;

        //These are used to account for the offset of the mouse click when targetting the move button (Stops Frame's topleft snapping to cursor)
        int MouseOffsetX; 
        int MouseOffsetY; 

        //Everything needed to draw all Frame information to the screen
        void Update(int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY);
        void DrawFrameBox(Color& FrameBoxColour);
        void AdjustFrame(Vector2 MouseXY);
        void DrawSingleColour(Color FillColour);
        int GetSmallestFrameSide(float SideX, float SideY);
        
    private:
        //For bounds checking on the Frames  
        Rectangle MainWindow; 
};      
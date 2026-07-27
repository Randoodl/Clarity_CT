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

#include "../include/ColourDropper.h"
#include <iostream>

ColourDropper::ColourDropper() 
{
    //public
    Visibility = true;
    MaxColours = {};
    CellFrameColour = {};
   
    //private
    StoredColours = {};
    DropperRectangles = {};
    DropperArea = {};
    

}


void ColourDropper::DrawDropper()
{
    for(int i_StoredColour {0}; i_StoredColour < MaxColours; ++ i_StoredColour)
    {
        //Draw the held colours
    }

    for(Rectangle DropperCell : DropperRectangles)
    {
        DrawRectangleLinesEx(DropperCell, 2, *CellFrameColour);
    }
    //DrawRectangle(DropperArea.x, DropperArea.y, DropperArea.width, DropperArea.height, GREEN);
}


void ColourDropper::Update(Rectangle SetDropperArea, bool SetVisibility)
{
    DropperArea = SetDropperArea;
    Visibility = SetVisibility;
}


void ColourDropper::GenerateStoredColours()
{
    //Create the vector wherein selected colours can be stored

    //Reset vector
    StoredColours.clear();
    StoredColours.reserve(MaxColours);

    //Populate the vector with dummy values of alpha 0
    for(int ColourIndex {0}; ColourIndex < MaxColours; ++ ColourIndex)
    {
        StoredColours.emplace_back((Color){0, 0, 0, 0});
    }
}


void ColourDropper::GenerateDropperRectangles()
{
    //Divide the total ColourDropper area into smaller rectangles that each represent one colour
    int TotalColours = StoredColours.size();

    //Reset Vector
    DropperRectangles.clear();
    DropperRectangles.reserve(TotalColours);

    //Populate the total frame area with rectangular sub-areas
    for(int Rects {0}; Rects < TotalColours; ++Rects)
    {
        Rectangle SetRectangle = {0, 0, 0, 0}; //Placeholder

        if(DropperArea.width >= DropperArea.height) //Sort horizontally
        {
            SetRectangle.x = DropperArea.x + (int(DropperArea.width / TotalColours) * Rects);
            SetRectangle.y = DropperArea.y;
            SetRectangle.width = int(DropperArea.width / TotalColours);
            SetRectangle.height = DropperArea.height;
        }
        else //Sort vertically
        {
            SetRectangle.x = DropperArea.x;
            SetRectangle.y = DropperArea.y + (int(DropperArea.height / TotalColours) * Rects);
            SetRectangle.width = DropperArea.width;
            SetRectangle.height = int(DropperArea.height / TotalColours);
        }
        DropperRectangles.emplace_back(SetRectangle);
    }
}
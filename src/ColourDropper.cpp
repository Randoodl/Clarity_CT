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
#include "../include/Constants.h"


ColourDropper::ColourDropper() 
{
    //public
    MaxColours = {};
    StoredColours = {};
    CellFrameColour = {};
    Visibility = true;
   
    //private
    DropperArea = {};
    DropperRectangles = {};
}


void ColourDropper::Update(Rectangle SetDropperArea, bool SetVisibility)
{
    DropperArea = SetDropperArea;
    Visibility = SetVisibility;
}


Color ColourDropper::GetDropperValue(Vector2 MouseXY)
{
    //Return the RGB values from a clicked-on DropperRectangle
    for(int i_DropperRectangle {0}; i_DropperRectangle < MaxColours; ++i_DropperRectangle)
    {
        if(CheckCollisionPointRec(MouseXY, DropperRectangles[i_DropperRectangle]))
        {
            return StoredColours[i_DropperRectangle];
        }
    }
    return BLANK;  //Default dummy value
}


void ColourDropper::DrawDropper()
{
    //Cycle through both DropperRectangles and StoredColours vectors to display a DropperRectangle with its paired colour
    for(int i_StoredColour {0}; i_StoredColour < MaxColours; ++ i_StoredColour)
    {
        DrawRectangle(DropperRectangles[i_StoredColour].x,
                      DropperRectangles[i_StoredColour].y,
                      DropperRectangles[i_StoredColour].width,
                      DropperRectangles[i_StoredColour].height,
                      StoredColours[i_StoredColour]);
    }

    //Overlay the colours with a visual frame, this is mostly to show "empty cells" when no colours are present yet
    for(Rectangle DropperCell : DropperRectangles)
    {
        DrawRectangleLinesEx(DropperCell, 2, *CellFrameColour);
    }
}


void ColourDropper::GenerateStoredColours()
{
    //Create the vector wherein selected colours can be stored

    //Keep track of already present colours in case of a UI reset
    std::vector<Color> PreservedColours {};

    for(Color ColourCell : StoredColours)
    {
        //If any Color in the StoredColours has a non-zero alpha, then it must already be populated
        if(ColourCell.a != RGBValMin)
        {
            PreservedColours = StoredColours;
            break;
        }
    }

    //Reset vector
    StoredColours.clear();
    StoredColours.reserve(MaxColours);

    //Potentially overwrite StoredColours with already present values
    if(PreservedColours.size())
    {
        StoredColours = PreservedColours;
    }
    else
    {
        //Populate the vector with dummy values
        for(int ColourIndex {0}; ColourIndex < MaxColours; ++ColourIndex)
        {
            StoredColours.emplace_back(BLANK);
        }
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
        Rectangle SetRectangle = {}; //Placeholder

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
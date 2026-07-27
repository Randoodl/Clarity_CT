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

#pragma once

#include <vector>
#include <raylib.h>


class ColourDropper
{
    //This class is meant to hold all the information for an Element to store colours for future use

    public:
        ColourDropper();
        int MaxColours;
        bool Visibility;
        Color* CellFrameColour;
        void DrawDropper();
        void Update(Rectangle SetDropperArea, bool SetVisibility);
        void GenerateStoredColours();
        void GenerateDropperRectangles();

    private:
        std::vector<Color> StoredColours;
        std::vector<Rectangle> DropperRectangles;
        Rectangle DropperArea;
};
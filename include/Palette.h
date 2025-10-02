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
#include <algorithm>
#include <map>

#include "./Constants.h"


class Palette
{
    //This class is meant to hold vectors of generated colours and linked rectangles to display them
    
    public:
        Palette();

        //The colour from which to generate the whole Palette
        Color BasePaletteColour;

        //How many bands of shades/tints to either side of the base colour and by how much should such a band vary in colour
        //This leaves room for customisation options later, if ever I want to implement that kind of functionality
        int VariationAmount;
        int VariationDelta;

        //Return the value of the clicked on colour band
        Color GetVariationColour(Vector2 MouseXY);

        //Everything needed to calculate and draw Palettes
        void Update(Rectangle SetPaletteArea, int SetVariationAmount, int SetVariationDelta);
        void SetHueShadePair(Color Hue, Color Shade);
        void GenerateShadesTints(Color SeedColour);
        void GeneratePaletteRectangles();
        void DrawPalette();

    private:
        Rectangle PaletteArea;

        //The smaller, subdivided, areas of the bar, or the bands that make up the whole Palette
        std::vector<Rectangle> PaletteRectangles;

        //The positionally linked colour for each band
        std::vector<Color> PaletteColours;
};
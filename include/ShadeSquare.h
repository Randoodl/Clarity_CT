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
#include <cmath>
#include <vector>

#include "./Constants.h"


class ShadeSquare
{
    //This class is meant to hold all the information for the shade square in the centre of the colour dial

    public:
        ShadeSquare();

        //Pubicly mutable attributes for calculating and drawing the ShadeSquare
        Rectangle ShadeSquareRectangle;
        Color SquareBaseColour;
        Color ShadedColour;

        //Everything related to generating the texture for the Shade Square
        bool ShadedImageIsLoaded; 
        Texture2D ShadedImage;
        std::vector<std::vector<Color>> GetVectorOfPixels();
        void ConvertVectorToTexture(const std::vector<std::vector<Color>>& VectorOfPixels);

        //These might be called upon by RGBDial so one updates the other and vice versa
        Vector2 CurrentShadeMouseLocation;
        Color GetSquareRGB(Vector2 MouseXY, Color SeedColour);

        void Update(Rectangle TotalFrameArea);
        void DrawShadeSquare();

    private:

        //Everything needed to calculate and draw an indicator on the ShadeSquare to show the current position
        float ShadeViewBoxDimensions;
        Vector2 ShadeViewBoxXY;
        Color ShadeViewBoxOutline;
        void UpdateShadeViewBoxPosition(Vector2 MouseXY);
        Vector2 GetCorrectedMouseXY(Vector2 MouseXY);
};
#pragma once

#include <raylib.h>
#include <vector>

#include "./Constants.h"

class Palette
{
    //This class is meant to hold all the information for a small collection of related colours
    public:
        Palette();
        Color BasePaletteColour;
        std::vector<Rectangle> PaletteSquares;
        std::vector<Color> PaletteSquareColours;
        Rectangle PaletteBar;

        Color GetVariationColour(Vector2 MouseXY);

        void Update(Rectangle PaletteArea, int Variations);
        void UpdateShadesTints(Color SeedColour, bool GenerateShades, int VariationDelta, int RGBLimit);

        void DrawPalette();

        Font SetFont;

    private:      
        int VariationAmount;
        
};
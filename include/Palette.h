#pragma once

#include <raylib.h>
#include <vector>
#include <algorithm>

#include "./Constants.h"

class Palette
{
    //This class is meant to hold all the information for a small collection of related colours
    public:
        Palette();
        Color BasePaletteColour;
        std::vector<Rectangle> PaletteRectangles;
        std::vector<Color> PaletteColours;
        Rectangle PaletteArea;

        Color GetVariationColour(Vector2 MouseXY);

        void Update(Rectangle SetPaletteArea, int SetVariationAmount, int SetVariationDelta);
        void GenerateShadesTints(Color SeedColour);
        void GeneratePaletteRectangles();

        void DrawPalette();

        Font SetFont;
             
        int VariationAmount;
        int VariationDelta;
        
};
#pragma once

#include <raylib.h>
#include <vector>
#include <algorithm>

#include "./Constants.h"


class Palette
{
    //This class is meant to hold vectors of generated colours and linked rectangles to display them
    
    public:
        Palette();
        Color BasePaletteColour;

        Color GetVariationColour(Vector2 MouseXY);

        void Update(Rectangle SetPaletteArea, int SetVariationAmount, int SetVariationDelta);
        void GenerateShadesTints(Color SeedColour);
        void GeneratePaletteRectangles();

        void DrawPalette();

        Font SetFont;

        int VariationAmount;
        int VariationDelta;
    
    private:
        Rectangle PaletteArea;
        std::vector<Rectangle> PaletteRectangles;
        std::vector<Color> PaletteColours;
};
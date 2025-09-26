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
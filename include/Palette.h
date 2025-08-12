#pragma once

#include <raylib.h>
#include <vector>

class Palette
{
    //This class is meant to hold all the information for a small collection of related colours
    public:
        Palette();
        Color BasePaletteColour;
        std::vector<Color> ColourRange;
        
        Rectangle ShadesAndTintsRectangle;
        void UpdateShadesTints(Color SeedColour, Rectangle SeedArea);
        void DrawShadesAndTints();

    private:      
        int ShadeAmount;
};
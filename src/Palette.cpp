#include "../include/Palette.h"

#include <iostream>

Palette::Palette()
{
    BasePaletteColour = {0, 0, 0, 255};
    ShadeAmount = 4; //Shades on either side of the BasePaletteColour
    ColourRange = {};
    ShadesAndTintsRectangle = {0, 0, 0, 0};
}


void Palette::UpdateShadesTints(Color SeedColour, Rectangle SeedArea)
{
    ShadesAndTintsRectangle.x = SeedArea.x;
    ShadesAndTintsRectangle.y = SeedArea.y;
    ShadesAndTintsRectangle.width = SeedArea.width;
    ShadesAndTintsRectangle.height = SeedArea.height;

    ColourRange = {};

    for(int ShadeNumber {0}; ShadeNumber < ShadeAmount; ++ShadeNumber)
    {   
        Color ShadeColour = SeedColour;

        ShadeColour.r = SeedColour.r - ShadeNumber * 10; 
        ShadeColour.g = SeedColour.g - ShadeNumber * 10;
        ShadeColour.b = SeedColour.b - ShadeNumber * 10;

        ColourRange.insert(ColourRange.begin(),ShadeColour);
    }
    ColourRange.emplace_back(SeedColour);
    for(int ShadeNumber {0}; ShadeNumber < ShadeAmount; ++ShadeNumber)
    {   
        Color ShadeColour = SeedColour;

        ShadeColour.r = SeedColour.r + ShadeNumber * 10; 
        ShadeColour.g = SeedColour.g + ShadeNumber * 10;
        ShadeColour.b = SeedColour.b + ShadeNumber * 10;

        ColourRange.emplace_back(ShadeColour);
    }
}


void Palette::DrawShadesAndTints()
{
    int WidLen = ShadesAndTintsRectangle.width / (1 + (ShadeAmount * 2));
    for(int i {0}; i < int(ColourRange.size()); ++i)
    {
        DrawRectangle(ShadesAndTintsRectangle.x + WidLen * i, ShadesAndTintsRectangle.y, WidLen, ShadesAndTintsRectangle.height, ColourRange[i]);
    }
}
#include "../include/Palette.h"

//#include <iostream>

Palette::Palette()
{
    BasePaletteColour = {0, 0, 0, 255};
    PaletteSquares = {};
    PaletteSquareColours = {};
    PaletteBar = {0, 0, 0, 0};

    VariationAmount = 0; 
    SetFont = GetFontDefault();
}


void::Palette::Update(Rectangle PaletteArea, int Variations)
{
    PaletteBar = PaletteArea;
    VariationAmount = Variations;
}


void Palette::UpdateShadesTints(Color SeedColour, bool GenerateShades, int VariationDelta, int RGBLimit)
{
    //Generate a vector of colours and a corresponding vector of rectangle areas
    //That represent either the shades or the tints (separate Palette instances) of the SeedColour based on GenerateShades' state

    //Reset vectors
    PaletteSquares.clear();
    PaletteSquares.reserve(VariationAmount);
    PaletteSquareColours.clear();
    PaletteSquareColours.reserve(VariationAmount);

    //Set Factors
    float FactorRed {};
    float FactorGreen {};
    float FactorBlue {};

    //This will follow the same logic as presented in ShadeSquare.cpp
    std::vector<float> RGBFloats = {float(SeedColour.r), float(SeedColour.g), float(SeedColour.b)};

    //The proportional difference from the R, G or B value to either 0 or 255, based on GenerateShades' state
    if(GenerateShades)
    {
        FactorRed   = RGBFloats[0] / 255.;
        FactorGreen = RGBFloats[1] / 255.;
        FactorBlue  = RGBFloats[2] / 255.;
    }
    else if(!GenerateShades)
    {
        FactorRed   = -((255 - RGBFloats[0]) / 255.); //These are negative to turn into additions when subtracting later
        FactorGreen = -((255 - RGBFloats[1]) / 255.);
        FactorBlue  = -((255 - RGBFloats[2]) / 255.);
    }

    //Generate a spread of variations 
    for(int Variation {0}; Variation < VariationAmount; ++Variation)
    {
        Color SetColour = {0, 0, 0, 255};    //Placeholder colour and rectangle to work with
        Rectangle SetRectangle {0, 0, 0, 0};

        //Update Variation's RGB values, keeping it between 0-255
        
        //RED
        if(RGBFloats[0] - (FactorRed * VariationDelta) >= 0 && RGBFloats[0] - (FactorRed * VariationDelta) <= 255)
        {
            RGBFloats[0] -= (FactorRed * VariationDelta);
        }
        else
        {
            RGBFloats[0] = RGBLimit;
        }

        //GREEN
        if(RGBFloats[1] - (FactorGreen * VariationDelta) >= 0 && RGBFloats[1] - (FactorGreen * VariationDelta) <= 255)
        {
            RGBFloats[1] -= (FactorGreen * VariationDelta);
        }
        else
        {
            RGBFloats[1] = RGBLimit;
        }

        //BLUE
        if(RGBFloats[2] - (FactorBlue * VariationDelta) >= 0 && RGBFloats[2] - (FactorBlue * VariationDelta) <= 255)
        {
            RGBFloats[2] -= (FactorBlue * VariationDelta);
        }
        else
        {
            RGBFloats[2] = RGBLimit;
        }

        //Update the colour and add to vector
        SetColour.r = RGBFloats[0];
        SetColour.g = RGBFloats[1];
        SetColour.b = RGBFloats[2];
        PaletteSquareColours[Variation] = SetColour;

        //Time to generate the Rectangle area which will contain the above calculated Color
        PaletteSquares[Variation] = SetRectangle;

        if(PaletteBar.width >= PaletteBar.height)  //Sort Horizontally
        {
            PaletteSquares[Variation].x = PaletteBar.x + (int(PaletteBar.width / VariationAmount) * Variation);
            PaletteSquares[Variation].y = PaletteBar.y + (!GenerateShades * (PaletteBar.height/2));
            PaletteSquares[Variation].width = int(PaletteBar.width / VariationAmount);
            PaletteSquares[Variation].height = PaletteBar.height / 2;
        }
        else //Sort Palette vertically
        {
            PaletteSquares[Variation].x = PaletteBar.x + (!GenerateShades * (PaletteBar.width/2));
            PaletteSquares[Variation].y = PaletteBar.y + (int(PaletteBar.height / VariationAmount) * Variation);
            PaletteSquares[Variation].width = PaletteBar.width/2;
            PaletteSquares[Variation].height = int(PaletteBar.height / VariationAmount);
        }
    } 
}


void Palette::DrawPalette()
{   
    for(int Variation {0}; Variation < VariationAmount; ++Variation)
    {
        Color GetColour = PaletteSquareColours[Variation];
        Rectangle GetRectangle = (PaletteSquares[Variation]);

        DrawRectangle(GetRectangle.x, GetRectangle.y, GetRectangle.width, GetRectangle.height, GetColour);
    }
}


Color Palette::GetVariationColour(Vector2 MouseXY)
{
    //Return the RGB values from a clicked-on Palette square
    for(int Variation {0}; Variation < VariationAmount; ++Variation)
    {
        Rectangle GetRectangle = (PaletteSquares[Variation]);
        if(CheckCollisionPointRec(MouseXY, GetRectangle))
        {
            Color GetColour = PaletteSquareColours[Variation];
            return GetColour;
        }
    }
    return {0, 0, 0, 0}; //Alpha value 0
}
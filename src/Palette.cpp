#include "../include/Palette.h"


Palette::Palette()
{
    BasePaletteColour = {0, 0, 0, 255};
    PaletteRectangles = {};
    PaletteColours = {};
    PaletteArea = {0, 0, 0, 0};
    VariationAmount = 0; 
}


void::Palette::Update(Rectangle SetPaletteArea, int SetVariationAmount, int SetVariationDelta)
{
    PaletteArea = SetPaletteArea;
    VariationAmount = SetVariationAmount;
    VariationDelta = SetVariationDelta;
}


void Palette::GenerateShadesTints(Color SeedColour)
{
    //Generate a vector of colours based on the SeedColour

    //Reset vector
    PaletteColours.clear();
    PaletteColours.reserve((VariationAmount * 2) + 1);

    //This controls whether to generate Shades or Tints
    bool GenerateShades = true;

    //Set Factors
    float FactorRed {};
    float FactorGreen {};
    float FactorBlue {};

    //Generate the Shade half of the spectrum first, then add the seed colour, then generate the Tint half
    for(int SpectrumHalf {0}; SpectrumHalf < 2; ++SpectrumHalf)
    {
        //This will follow the same logic as presented in ShadeSquare.cpp
        float RGBFloats[3] = {float(SeedColour.r), float(SeedColour.g), float(SeedColour.b)};

        //The proportional difference from the R, G or B value to either 0 or 255, based on GenerateShades' state
        if(GenerateShades)
        {
            FactorRed   = RGBFloats[0] / float(RGBValMax);
            FactorGreen = RGBFloats[1] / float(RGBValMax);
            FactorBlue  = RGBFloats[2] / float(RGBValMax);
        }
        else if(!GenerateShades)
        {
            //These are negative to turn into additions when subtracting later
            FactorRed   = -((RGBValMax - RGBFloats[0]) / float(RGBValMax)); 
            FactorGreen = -((RGBValMax - RGBFloats[1]) / float(RGBValMax));
            FactorBlue  = -((RGBValMax - RGBFloats[2]) / float(RGBValMax));
        }

        //Generate a spread of variations 
        for(int Variation {0}; Variation < VariationAmount; ++Variation)
        {
            Color SetColour = {0, 0, 0, 255};    //Placeholder colour to work with

            //Update Variation's RGB values, keeping it between 0-255

            //RED
            if(RGBFloats[0] - (FactorRed * VariationDelta) >= RGBValMin && RGBFloats[0] - (FactorRed * VariationDelta) <= RGBValMax)
            {
                RGBFloats[0] -= (FactorRed * VariationDelta);
            }
            else
            {
                RGBFloats[0] = (!GenerateShades * RGBValMax); //0 if shades, 255 if not shades
            }

            //GREEN
            if(RGBFloats[1] - (FactorGreen * VariationDelta) >= RGBValMin && RGBFloats[1] - (FactorGreen * VariationDelta) <= RGBValMax)
            {
                RGBFloats[1] -= (FactorGreen * VariationDelta);
            }
            else
            {
                RGBFloats[1] = (!GenerateShades * RGBValMax);
            }

            //BLUE
            if(RGBFloats[2] - (FactorBlue * VariationDelta) >= RGBValMin && RGBFloats[2] - (FactorBlue * VariationDelta) <= RGBValMax)
            {
                RGBFloats[2] -= (FactorBlue * VariationDelta);
            }
            else
            {
                RGBFloats[2] = (!GenerateShades * RGBValMax);
            }

            //Update the colour and add to vector
            SetColour.r = RGBFloats[0];
            SetColour.g = RGBFloats[1];
            SetColour.b = RGBFloats[2];
            PaletteColours.emplace_back(SetColour);
        } 
        
        if(GenerateShades)
        {
            //Reverse the vector so far, so it runs dark to light, add a seed colour 'middle point'
            std::reverse(PaletteColours.begin(), PaletteColours.end());
            PaletteColours.emplace_back(SeedColour);
            GenerateShades = false;
        }
    }
    
    //This gives a neat little "source colour" block at the start of the palette
    PaletteColours.insert(PaletteColours.begin(), {SeedColour, SeedColour, BLANK});
}


void Palette::SetHueShadePair(Color Hue, Color Shade)
{
    //Generates a small tw-colour palette to display a Hue and its Shade/Tint

    //Reset vector
    PaletteColours.clear();
    PaletteColours.reserve(2);

    //This should be one call
    PaletteColours.emplace_back(Hue);
    PaletteColours.emplace_back(Shade);  
}

void Palette::GeneratePaletteRectangles()
{
    //Divide the total Palette area into smaller rectangles that each represent one colour
    int TotalColours = PaletteColours.size();

    //Reset Vector
    PaletteRectangles.clear();
    PaletteRectangles.reserve(TotalColours);

    //Generate rectangle areas relative to how many colours there are to represent
    for(int Variation {0}; Variation < TotalColours; ++Variation)
    {
        Rectangle SetRectangle = {0, 0, 0, 0};  //Placeholder rectangle to work with

        if(PaletteArea.width >= PaletteArea.height) //Sort horizontally
        {
            SetRectangle.x = PaletteArea.x + (int(PaletteArea.width / TotalColours) * Variation);
            SetRectangle.y = PaletteArea.y;
            SetRectangle.width = int(PaletteArea.width / TotalColours);
            SetRectangle.height = PaletteArea.height;
        }
        else //Sort vertically
        {
            SetRectangle.x = PaletteArea.x;
            SetRectangle.y = PaletteArea.y + (int(PaletteArea.height / TotalColours) * Variation);
            SetRectangle.width = PaletteArea.width;
            SetRectangle.height = int(PaletteArea.height / TotalColours);
        }

        PaletteRectangles.emplace_back(SetRectangle);
        
    }

    //Fix leftover gap at the right/bottom side of the Palette as result of floats narrowing
    //It's clumsy, but it gets the job done
    if(PaletteArea.width >= PaletteArea.height)
    {
        PaletteRectangles.back().width = PaletteArea.width - (PaletteRectangles.back().x - PaletteArea.x);
    }
    else
    {
        PaletteRectangles.back().height = PaletteArea.height - (PaletteRectangles.back().y - PaletteArea.y);
    }
}


void Palette::DrawPalette()
{       
    for(int Variation {0}; Variation < int(PaletteColours.size()); ++Variation)
    {
        Rectangle GetRectangle = PaletteRectangles[Variation];
        DrawRectangle(GetRectangle.x, GetRectangle.y, GetRectangle.width, GetRectangle.height, PaletteColours[Variation]);
    }
}


Color Palette::GetVariationColour(Vector2 MouseXY)
{
    //Return the RGB values from a clicked-on Palette square
    for(int Variation {0}; Variation < int(PaletteColours.size()); ++Variation)
    {
        if(CheckCollisionPointRec(MouseXY, PaletteRectangles[Variation]))
        {
            return PaletteColours[Variation];
        }
    }
    return {0, 0, 0, 0}; //Alpha value 0 for sentinel purposes
}
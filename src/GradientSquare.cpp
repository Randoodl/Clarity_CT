#include "../include/GradientSquare.h"
#include <iostream>

GradientSquare::GradientSquare()
{
    SquareBaseColour = {255, 0, 0, 255};
}


void GradientSquare::Update(int OffsetX, int OffsetY, int DialInnerRadius)
{
    /*
    Essentially, from the centre off the RGB Dial, how much do I offset XY
    To get a corner coordinate that touches the circle of the inner dial.
    Think of it as drawing a triangle from the centre of the dial with InnerRadius as the hypo
    the straight sides are then the length of half the square
    */

    double AnchorPointOffset = sqrt(0.5 * pow(DialInnerRadius, 2));
    SquareEdgeLength = 2 * AnchorPointOffset;  
    XAnchorPoint = OffsetX - AnchorPointOffset;
    YAnchorPoint = OffsetY - AnchorPointOffset;

    ColouredSquare.x = XAnchorPoint;
    ColouredSquare.y = YAnchorPoint;
    ColouredSquare.height = ColouredSquare.width = SquareEdgeLength;

}


void GradientSquare::DrawGradientSquare()
{
    //This seems like a very heavy-handed approach to do something as seemingly simple as a gradient square
    //doing 255^2 calculations each frame seems silly at best
    //There has to be a better way of doing this, but it is solid for now

    //How much EdgeLength per one of the 255 colours
    float EdgePerBand = float(SquareEdgeLength / 255.);
    float BandThickness = EdgePerBand;
    if(BandThickness < 1){BandThickness = 1;} //Square stops being visible with thicknessess under 1

    //Create a colour to be darkened
    Color DarkenRGBColour = SquareBaseColour;

    //Create a vector to hold the colour as floats (to avoid narrowing fuckery)
    std::vector<float> DarkRGBFloats = {float(DarkenRGBColour.r), float(DarkenRGBColour.g), float(DarkenRGBColour.b)};

    //The proportional difference from the R, G or B value to 0, per 1 step in 255 total steps
    float DarkFactorRed   = DarkRGBFloats[0] / 255.;
    float DarkFactorGreen = DarkRGBFloats[1] / 255.;
    float DarkFactorBlue  = DarkRGBFloats[2] / 255.;

    for(int DarkStep {0}; DarkStep < 255; ++DarkStep)
    {
        //Update the row to be slightly darker than the last one
        DarkRGBFloats[0] -= DarkFactorRed;  
        DarkRGBFloats[1] -= DarkFactorGreen;  
        DarkRGBFloats[2] -= DarkFactorBlue;   
        
        DarkenRGBColour.r = DarkRGBFloats[0];
        DarkenRGBColour.g = DarkRGBFloats[1];
        DarkenRGBColour.b = DarkRGBFloats[2];

        //Create a colour to be whitened
        Color LightenRGBColour = DarkenRGBColour;

        //Create a vector to hold the colour as floats
        std::vector<float> LightRGBFloats = {float(LightenRGBColour.r), float(LightenRGBColour.g), float(LightenRGBColour.b)};

        //The proportional difference from the R, G or B value to the highest RGB (255 - Darkstep), per 1 step in 255 total steps
        float LightFactorRed   = ((255 - DarkStep) - LightRGBFloats[0]) / 255;
        float LightFactorGreen = ((255 - DarkStep) - LightRGBFloats[1]) / 255;
        float LightFactorBlue  = ((255 - DarkStep) - LightRGBFloats[2]) / 255;

        for(int LightStep {0}; LightStep < 255; ++LightStep)
        {
            //Update the Row-Column pair (pixel) to be slightly lighter
            LightRGBFloats[0] += LightFactorRed;
            LightRGBFloats[1] += LightFactorGreen;
            LightRGBFloats[2] += LightFactorBlue;

            LightenRGBColour.r = LightRGBFloats[0];
            LightenRGBColour.g = LightRGBFloats[1];
            LightenRGBColour.b = LightRGBFloats[2];

            DrawRectangle(XAnchorPoint + (EdgePerBand * LightStep), YAnchorPoint + (EdgePerBand * DarkStep), std::ceil(BandThickness), std::ceil(BandThickness), LightenRGBColour);
        }               
    }
}


Color GradientSquare::GetSquareRGB(Vector2 MouseXY)
{
    return BLANK;
}

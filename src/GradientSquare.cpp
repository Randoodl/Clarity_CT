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

    //Create a vector to hold the colour as floats (to avoid narrowing fuckery)
    std::vector<float> DarkRGBFloats = {float(SquareBaseColour.r), float(SquareBaseColour.g), float(SquareBaseColour.b)};

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

        //Create an empty colour to store the per-pixel colour value
        Color GradientColour = {0,0,0,255};

        //Create a vector to hold the colour as floats
        std::vector<float> LightRGBFloats = {float(DarkRGBFloats[0]), float(DarkRGBFloats[1]), float(DarkRGBFloats[2])};

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

            GradientColour.r = LightRGBFloats[0];
            GradientColour.g = LightRGBFloats[1];
            GradientColour.b = LightRGBFloats[2];

            //Temporary debugger to check colour values visually
            DrawRectangle(XAnchorPoint + (EdgePerBand * LightStep), YAnchorPoint + (EdgePerBand * DarkStep), std::ceil(BandThickness), std::ceil(BandThickness), GradientColour);
        }               
    }
}


Color GradientSquare::GetSquareRGB(Vector2 MouseXY)
{
    //Turn the mouse's XY coordinates back into a colour doing a bunch of math translations
    //Essentially the inverse of DrawGradientSquare()

    //Color to update and return
    Color CalculatedColour = SquareBaseColour;

    //The mouse distance within the square translated to a value between 0-255, from top left to bottom right
    int RelativeX = ((MouseXY.x - XAnchorPoint) / SquareEdgeLength) * 255.;
    int RelativeY = ((MouseXY.y - YAnchorPoint) / SquareEdgeLength) * 255.;

    //Updating the colour in the Y direction, making it lighter
    float LightFactorRed   = ((255. - CalculatedColour.r) / 255.);
    float LightFactorGreen = ((255. - CalculatedColour.g) / 255.);
    float LightFactorBlue  = ((255. - CalculatedColour.b) / 255.);

    CalculatedColour.r += (LightFactorRed * RelativeX);
    CalculatedColour.g += (LightFactorGreen * RelativeX);
    CalculatedColour.b += (LightFactorBlue * RelativeX);

    //Updating the colour in the X direction, making it darker
    float DarkFactorRed   = (CalculatedColour.r / 255.);
    float DarkFactorGreen = (CalculatedColour.g / 255.);
    float DarkFactorBlue  = (CalculatedColour.b / 255.);

    CalculatedColour.r -= (DarkFactorRed * RelativeY);
    CalculatedColour.g -= (DarkFactorGreen * RelativeY);
    CalculatedColour.b -= (DarkFactorBlue * RelativeY);
    
    return CalculatedColour;
}

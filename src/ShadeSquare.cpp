#include "../include/ShadeSquare.h"


ShadeSquare::ShadeSquare()
{
    SquareBaseColour = {255, 0, 0, 255};
}


void ShadeSquare::Update(int OffsetX, int OffsetY, int DialInnerRadius)
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


void ShadeSquare::DrawShadeSquare()
{
    //This seems like a very heavy-handed approach to do something as seemingly simple as a shade square
    //doing 255^2 calculations each frame seems silly at best
    //There has to be a better way of doing this, but it is solid for now

    //How much EdgeLength per one of the 255 colours
    float EdgePerBand = float(SquareEdgeLength / 255.);
    float BandThickness = EdgePerBand;
    if(BandThickness < 1){BandThickness = 1;} //Square stops being visible with thicknessess under 1

    //Create a vector to hold the colour as floats (to avoid narrowing fuckery)
    std::vector<float> ShadeRGBFloats = {float(SquareBaseColour.r), float(SquareBaseColour.g), float(SquareBaseColour.b)};

    //The proportional difference from the R, G or B value to 0, per 1 step in 255 total steps
    float ShadingFactorRed   = ShadeRGBFloats[0] / 255.;
    float ShadingFactorGreen = ShadeRGBFloats[1] / 255.;
    float ShadingFactorBlue  = ShadeRGBFloats[2] / 255.;

    for(int ShaderStep {0}; ShaderStep < 255; ++ShaderStep)
    {
        //Update the row to be slightly darker than the last one
        ShadeRGBFloats[0] -= ShadingFactorRed;  
        ShadeRGBFloats[1] -= ShadingFactorGreen;  
        ShadeRGBFloats[2] -= ShadingFactorBlue;   

        //Create an empty colour to store the per-pixel colour value
        Color ShadeColour = {0,0,0,255};

        //Create a vector to hold the colour as floats
        std::vector<float> TintRGBFloats = {float(ShadeRGBFloats[0]), float(ShadeRGBFloats[1]), float(ShadeRGBFloats[2])};

        //The proportional difference from the R, G or B value to the highest RGB (255 - Darkstep), per 1 step in 255 total steps
        float TintingFactorRed   = ((255 - ShaderStep) - TintRGBFloats[0]) / 255;
        float TintingFactorGreen = ((255 - ShaderStep) - TintRGBFloats[1]) / 255;
        float TintingFactorBlue  = ((255 - ShaderStep) - TintRGBFloats[2]) / 255;

        for(int TinterStep {0}; TinterStep < 255; ++TinterStep)
        {
            //Update the Row-Column pair (pixel) to be slightly lighter
            TintRGBFloats[0] += TintingFactorRed;
            TintRGBFloats[1] += TintingFactorGreen;
            TintRGBFloats[2] += TintingFactorBlue;

            ShadeColour.r = TintRGBFloats[0];
            ShadeColour.g = TintRGBFloats[1];
            ShadeColour.b = TintRGBFloats[2];

            DrawRectangle(XAnchorPoint + (EdgePerBand * TinterStep), YAnchorPoint + (EdgePerBand * ShaderStep), std::ceil(BandThickness), std::ceil(BandThickness), ShadeColour);
        }               
    }
}


Color ShadeSquare::GetSquareRGB(Vector2 MouseXY)
{
    //Turn the mouse's XY coordinates back into a colour doing a bunch of math translations
    //Essentially the inverse of DrawShadeSquare()

    //Storing this for Dial feedback use
    CurrentShadeMouseLocation = MouseXY;

    //Color to update and return
    Color CalculatedColour = SquareBaseColour;

    //The mouse distance within the square translated to a value between 0-255, from top left to bottom right
    int RelativeX = ((MouseXY.x - XAnchorPoint) / SquareEdgeLength) * 255.;
    int RelativeY = ((MouseXY.y - YAnchorPoint) / SquareEdgeLength) * 255.;

    //Updating the colour in the Y direction, making it lighter (tint)
    float TintingFactorRed   = ((255. - CalculatedColour.r) / 255.);
    float TintingFactorGreen = ((255. - CalculatedColour.g) / 255.);
    float TintingFactorBlue  = ((255. - CalculatedColour.b) / 255.);

    CalculatedColour.r += (TintingFactorRed * RelativeX);
    CalculatedColour.g += (TintingFactorGreen * RelativeX);
    CalculatedColour.b += (TintingFactorBlue * RelativeX);

    //Updating the colour in the X direction, making it darker (shade)
    float ShadingFactorRed   = (CalculatedColour.r / 255.);
    float ShadingFactorGreen = (CalculatedColour.g / 255.);
    float ShadingFactorBlue  = (CalculatedColour.b / 255.);

    CalculatedColour.r -= (ShadingFactorRed * RelativeY);
    CalculatedColour.g -= (ShadingFactorGreen * RelativeY);
    CalculatedColour.b -= (ShadingFactorBlue * RelativeY);
    
    return CalculatedColour;
}
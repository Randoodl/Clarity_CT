#include "../include/GradientSquare.h"

GradientSquare::GradientSquare()
{
    XAnchorPoint = 0;
    YAnchorPoint = 0;
    SquareEdgeLength = 0;
    SquareBaseColour = {255, 0, 0, 255};
}


void GradientSquare::Initialise(int OffsetX, int OffsetY, int DialInnerRadius)
{
    /*
    Essentially, from the centre off the RGB Dial, how much do I offset XY
    To get a corner coordinate that touches the circle of the inner dial.
    Think of it as drawing a triangle from the centre of the dial with InnerRadius as the hypo
    the straight sides are then the length of half the square
    */

    double AnchorPointOffset = std::sqrt(0.5 * pow(DialInnerRadius, 2));
    XAnchorPoint = OffsetX - AnchorPointOffset;
    YAnchorPoint = OffsetY - AnchorPointOffset;
    SquareEdgeLength = 2 * AnchorPointOffset;  
}

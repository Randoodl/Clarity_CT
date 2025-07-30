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


void GradientSquare::DrawSquareGradient()
{
    DrawRectangleGradientEx(ColouredSquare, SquareBaseColour, BLACK, {127, 127, 127, 255}, WHITE);
}


Color GradientSquare::GetSquareRGB(Vector2 MouseXY)
{
    //Please don't look at this
    //Very much still a complete concept, but it works!

    //DEFINITELY reworking this, I mean, fuck, LOOK AT IT

    Color GradientColour = SquareBaseColour;

    float WhiteGrade = ((MouseXY.x - XAnchorPoint) / SquareEdgeLength);
    float BlackGrade = ((MouseXY.y - YAnchorPoint) / SquareEdgeLength);
    float TotalGrade = WhiteGrade + BlackGrade;

    float WhiteCorrection = (WhiteGrade / TotalGrade) * ((MouseXY.x - XAnchorPoint) / SquareEdgeLength);
    float BlackCorrection = (BlackGrade / TotalGrade) * ((MouseXY.y - YAnchorPoint) / SquareEdgeLength);

    float RFacW = (255. - SquareBaseColour.r) ; 
    float GFacW = (255. - SquareBaseColour.g) ; 
    float BFacW = (255. - SquareBaseColour.b) ; 

    float RFacB = SquareBaseColour.r; 
    float GFacB = SquareBaseColour.g;
    float BFacB = SquareBaseColour.b;     

    GradientColour.r = GradientColour.r + (RFacW * WhiteCorrection); 
    GradientColour.g = GradientColour.g + (GFacW * WhiteCorrection); 
    GradientColour.b = GradientColour.b + (BFacW * WhiteCorrection); 

    GradientColour.r = GradientColour.r - (RFacB * BlackCorrection); 
    GradientColour.g = GradientColour.g - (GFacB * BlackCorrection); 
    GradientColour.b = GradientColour.b - (BFacB * BlackCorrection); 

    return GradientColour;
}

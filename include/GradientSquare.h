#pragma once

#include "./ColourDial.h"

class GradientSquare
{
    //This class is meant to hold all the information for the gradient square in the centre of the colour dial

    public:
        GradientSquare();
        Color SquareBaseColour;
        void Initialise(int OffsetX, int OffsetY, int DialInnerRadius);
        void DrawGradientSquare();
    
    private:
        int XAnchorPoint;
        int YAnchorPoint;
        int SquareEdgeLength;


};
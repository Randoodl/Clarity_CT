#pragma once

#include <raylib.h>
#include <cmath>
#include <vector>

class GradientSquare
{
    //This class is meant to hold all the information for the gradient square in the centre of the colour dial

    public:
        GradientSquare();
        int XAnchorPoint;
        int YAnchorPoint;
        int SquareEdgeLength;

        Color SquareBaseColour;

        void Update(int OffsetX, int OffsetY, int DialInnerRadius);
        void DrawSquareGradient();
        Color GetSquareRGB(Vector2 MouseXY);

    private:
        Rectangle ColouredSquare;
};
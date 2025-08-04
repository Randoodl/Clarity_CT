#pragma once

#include <raylib.h>
#include <cmath>
#include <vector>

class ShadeSquare
{
    //This class is meant to hold all the information for the shade square in the centre of the colour dial

    public:
        ShadeSquare();
        int XAnchorPoint;
        int YAnchorPoint;
        int SquareEdgeLength;

        Color SquareBaseColour;

        void Update(Rectangle TotalFrameArea);
        void DrawShadeSquare();
        Color GetSquareRGB(Vector2 MouseXY);

        Vector2 CurrentShadeMouseLocation;
};
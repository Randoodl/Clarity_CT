#pragma once

#include <raylib.h>
#include <vector> 

class Frames
{
    //This class is meant to create the "windows" to move and scale elements
    
    public:
        Frames();

        int AnchorX;
        int AnchorY;
        int LenX;
        int LenY;

        Rectangle FrameArea;
        Rectangle MoveButton;
        Rectangle ScaleButton;

        void Update(int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY);
        void DrawFrameBox();
        void AdjustFrame(Vector2 MouseXY, float Ratio);
        void SetFrameRatio(double RatioXY);
        int EdgeButtonSize;
};      
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

        int EdgeButtonSize;
        std::vector<int> MoveButtonRoot;  //XY Position of the Move "Button" in the Frame
        std::vector<int> ScaleButtonRoot; //Ditto for the Scale "Button"

        void Initialise(int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY);
        void DrawFrameBox();
        void SetFrameRatio(double RatioXY);

        //Add Move and Scale
};      
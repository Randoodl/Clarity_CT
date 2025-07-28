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
        std::vector<int> MoveButtonRoot;
        std::vector<int> ScaleButtonRoot;


        void Initialise(int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY);
        void DrawFrameBox();

        //Add Move and Scale
};      
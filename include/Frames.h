#pragma once

#include <raylib.h>
#include <vector> 

class Frames
{
    //This class is meant to create the "windows" to move and scale elements
    
    public:
        Frames();
        Rectangle FrameArea;
        Rectangle MoveButton;
        Rectangle ScaleButton;

        void Update(int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY);
        void DrawFrameBox();
        void AdjustFrame(Vector2 MouseXY);
        int GetSmallestFrameSide(float SideX, float SideY);
        int EdgeButtonSize;

        bool ActiveFrame;
        bool IsDragging;
        bool IsScaling;

    private:
        Rectangle MainWindow; //For bounds checking on the boxes
        
};      
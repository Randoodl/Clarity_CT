#include "../include/Frames.h"
#include <iostream>

Frames::Frames()
{
    FrameArea = {0, 0, 0, 0};
    MoveButton = {0, 0, 0, 0};
    ScaleButton = {0, 0, 0, 0};
    EdgeButtonSize = 20;
    ActiveFrame = false;
    IsDragging = false;
    IsScaling = false;
    MouseOffsetX = 0;
    MouseOffsetY = 0;
    MainWindow = {0, 0, float(GetScreenWidth()), float(GetScreenHeight())};
}


void Frames::Update(int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY)
{
    //Update all position and size data when frame is moved or scaled
    FrameArea.width = SetLenX;
    FrameArea.height = SetLenY;
    FrameArea.x = SetAnchorX;
    FrameArea.y = SetAnchorY;

    //This caps how small the frame can possibly be: stops Scale and Move button from overlapping
    if(FrameArea.width < (2 * EdgeButtonSize)){FrameArea.width = (2 * EdgeButtonSize);}
    if(FrameArea.height < (2 * EdgeButtonSize)){FrameArea.height = (2 * EdgeButtonSize);}

    MoveButton.height =  EdgeButtonSize;
    MoveButton.width = FrameArea.width;
    MoveButton.x = FrameArea.x;
    MoveButton.y = FrameArea.y;

    ScaleButton.height = ScaleButton.width = EdgeButtonSize;
    ScaleButton.x = FrameArea.width + FrameArea.x - EdgeButtonSize;
    ScaleButton.y = FrameArea.height + FrameArea.y - EdgeButtonSize;

    MainWindow = {0, 0, float(GetScreenWidth()), float(GetScreenHeight())};
}


void Frames::DrawFrameBox()
{
    //Draw the Frame surrounding the element, as well as the two corner button areas
    DrawRectangleLines(FrameArea.x, FrameArea.y, FrameArea.width, FrameArea.height, RAYWHITE);
    DrawRectangle(MoveButton.x, MoveButton.y, MoveButton.width, MoveButton.height, RAYWHITE);
    DrawRectangle(ScaleButton.x, ScaleButton.y, ScaleButton.width, ScaleButton.height, RAYWHITE);
} 


void Frames::AdjustFrame(Vector2 MouseXY)
{
    //Each cycle while interactible bool is true:
    //    Move or Scale frame
    //    Check for out-of-bounds condition
    //      >Snap frame back to Window

    Vector2 MouseXYDelta = GetMouseDelta();

    //Update MouseXY to handle the click relative to the topleft point of the button
    MouseXY.x += MouseOffsetX;
    MouseXY.y += MouseOffsetY;

    if(IsDragging)
    {
        Update(MouseXY.x, MouseXY.y, FrameArea.width, FrameArea.height);

        if(!CheckCollisionPointRec({FrameArea.x, FrameArea.y}, MainWindow))  //TopLeft is out of bounds
        {
            Update((MouseXY.x > 0) * MouseXY.x, (MouseXY.y > 0) * MouseXY.y, FrameArea.width, FrameArea.height); //Your dumbest idea yet, but it works
        }
        if(!CheckCollisionPointRec({FrameArea.x + FrameArea.width, FrameArea.y + FrameArea.height}, MainWindow)) //BottomRight is out of bounds
        {
            //I take it back, THIS is your dumbest idea yet, but it somehow ALSO works
            //It combines the three possibilities of out-of-bounds (x; y; x&y) into one Update call
            //  IF a point is out of bounds, the left side of the calculation will trigger, calling Update with:   (1 * max position) + (0 * current position)
            //  IF a point is within bounds, the right side of the calculation will trigger, calling Update with:  (0 * max position) + (1 * current position)
            //notice that this calculation is mutually exclusive
            Update(
            (((FrameArea.x + FrameArea.width) > MainWindow.width) * (MainWindow.width - FrameArea.width)) + (((FrameArea.x + FrameArea.width) <= MainWindow.width) * FrameArea.x), 
            (((FrameArea.y + FrameArea.height) > MainWindow.height) * (MainWindow.height - FrameArea.height)) + (((FrameArea.y + FrameArea.height) <= MainWindow.height) * FrameArea.y),  
            FrameArea.width,
            FrameArea.height);    
        }
    }
    if(IsScaling)
    {
        Update(FrameArea.x, FrameArea.y, FrameArea.width + MouseXYDelta.x, FrameArea.height + MouseXYDelta.y);

        if(!CheckCollisionPointRec({FrameArea.x + FrameArea.width, FrameArea.y + FrameArea.height}, MainWindow)) //BottomRight is out of bounds
        {
            //Similar logic to the BottomRight bounds correction above, just adapted for the width/height instead of the X/Y anchor coordinate
            Update(FrameArea.x, FrameArea.y, 
            (((FrameArea.x + FrameArea.width) > MainWindow.width) * (MainWindow.width - FrameArea.x) + ((FrameArea.x + FrameArea.width) <= MainWindow.width) * FrameArea.width),
            (((FrameArea.y + FrameArea.height) > MainWindow.height) * (MainWindow.height - FrameArea.y) + ((FrameArea.y + FrameArea.height) <= MainWindow.height) * FrameArea.height)
            );
        }
    }
}


int Frames::GetSmallestFrameSide(float SideX, float SideY)
{
    return std::min(SideX, SideY);
}

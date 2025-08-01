#include "../include/Frames.h"

Frames::Frames()
{
    EdgeButtonSize = 40;
}


void Frames::Update(int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY)
{
    //Update all position and size data when frame is moved or scaled

    AnchorX = SetAnchorX;
    AnchorY = SetAnchorY;
    LenX = SetLenX;
    LenY = SetLenY;

    FrameArea.width = SetLenX;
    FrameArea.height = SetLenY;
    FrameArea.x = SetAnchorX;
    FrameArea.y = SetAnchorY;

    MoveButton.height = MoveButton.width = EdgeButtonSize;
    MoveButton.x = AnchorX;
    MoveButton.y = AnchorY;

    ScaleButton.height = ScaleButton.width = EdgeButtonSize;
    ScaleButton.x = LenX + AnchorX - EdgeButtonSize;
    ScaleButton.y = LenY + AnchorY - EdgeButtonSize;
}


void Frames::DrawFrameBox()
{
    //Draw the Frame surrounding the element, as well as the two corner button areas
    DrawRectangleLines(AnchorX, AnchorY, LenX, LenY, RAYWHITE);
    DrawRectangle(MoveButton.x, MoveButton.y, EdgeButtonSize, EdgeButtonSize, RAYWHITE);
    DrawRectangle(ScaleButton.x, ScaleButton.y, EdgeButtonSize, EdgeButtonSize, RAYWHITE);
} 


void Frames::AdjustFrame(Vector2 MouseXY, float Ratio)
{

    Vector2 MouseXYDelta = GetMouseDelta();

    if (CheckCollisionPointRec(MouseXY, MoveButton))
    {   
        //The click occurs on the move button
        Update(MouseXYDelta.x + AnchorX, MouseXYDelta.y + AnchorY, LenX , LenY);
    }
    else if (CheckCollisionPointRec(MouseXY, ScaleButton))
    {
        //The click occurs on the scale button
        Update(AnchorX, AnchorY, LenX + MouseXYDelta.x, LenY + MouseXYDelta.y);
        
        SetFrameRatio(Ratio); //Rework this, it is a silly way to force a ratio
    }
}


void Frames::SetFrameRatio(double RatioXY)
{
    //This is a stupid, but for the time easy way to force a ratio on frames
    LenY = LenX * RatioXY;
    ScaleButton.x = LenX + AnchorX - EdgeButtonSize;
    ScaleButton.y = LenY + AnchorY - EdgeButtonSize;
}

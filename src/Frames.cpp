#include "../include/Frames.h"

Frames::Frames()
{
    AnchorX = 0;
    AnchorY = 0;
    LenX = 0;
    LenY = 0;
    EdgeButtonSize = 40;
}


void Frames::Update(int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY)
{
    AnchorX = SetAnchorX;
    AnchorY = SetAnchorY;
    LenX = SetLenX;
    LenY = SetLenY;

    MoveButton.height = MoveButton.width = EdgeButtonSize;
    MoveButton.x = AnchorX;
    MoveButton.y = AnchorY;

    ScaleButton.height = ScaleButton.width = EdgeButtonSize;
    ScaleButton.x = LenX + AnchorX - EdgeButtonSize;
    ScaleButton.y = LenY + AnchorY - EdgeButtonSize;
}


void Frames::DrawFrameBox()
{
    DrawRectangleLines(AnchorX, AnchorY, LenX, LenY, RAYWHITE);
    DrawRectangle(MoveButton.x, MoveButton.y, EdgeButtonSize, EdgeButtonSize, RAYWHITE);
    DrawRectangle(ScaleButton.x, ScaleButton.y, EdgeButtonSize, EdgeButtonSize, RAYWHITE);
} 


void Frames::SetFrameRatio(double RatioXY)
{
    //This is a stupid, but for the time easy way to force a ratio on frames
    LenY = LenX * RatioXY;
    ScaleButton.x = LenX + AnchorX - EdgeButtonSize;
    ScaleButton.y = LenY + AnchorY - EdgeButtonSize;
}

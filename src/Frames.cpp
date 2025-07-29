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

    MoveButtonRoot = {AnchorX, AnchorY};
    ScaleButtonRoot = {LenX + AnchorX - EdgeButtonSize, LenY + AnchorY - EdgeButtonSize};
}


void Frames::DrawFrameBox()
{
    DrawRectangleLines(AnchorX, AnchorY, LenX, LenY, RAYWHITE);
    DrawRectangle(MoveButtonRoot[0], MoveButtonRoot[1], EdgeButtonSize, EdgeButtonSize, RAYWHITE);
    DrawRectangle(ScaleButtonRoot[0], ScaleButtonRoot[1], EdgeButtonSize, EdgeButtonSize, RAYWHITE);
}


void Frames::SetFrameRatio(double RatioXY)
{
    //This is a stupid, but for the time easy way to force a ratio on frames
    LenY = LenX * RatioXY;
    ScaleButtonRoot = {LenX + AnchorX - EdgeButtonSize, LenY + AnchorY - EdgeButtonSize};
}

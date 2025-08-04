#include "../include/Frames.h"

Frames::Frames()
{
    EdgeButtonSize = 20;
    MainWindow = {0, 0, float(GetScreenWidth()), float(GetScreenHeight())};
    ActiveFrame = false;
    IsDragging = false;
    IsScaling = false;
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


void Frames::AdjustFrame(Vector2 MouseXY)
{

    Vector2 MouseXYDelta = GetMouseDelta();

    if (IsDragging) //The click occurs on the move button
    {
        //Stops it from being dragged off-screen, but feels kind of clunky?
        if(CheckCollisionPointRec({AnchorX + MouseXYDelta.x, AnchorY + MouseXYDelta.y}, MainWindow) && 
           CheckCollisionPointRec({ScaleButton.x + ScaleButton.width + MouseXYDelta.x, ScaleButton.y + ScaleButton.height + MouseXYDelta.y}, MainWindow)) 
        {
            Update(MouseXYDelta.x + AnchorX, MouseXYDelta.y + AnchorY, LenX , LenY);
        }

    }
    else if (IsScaling)  //The click occurs on the scale button
    {
        //Not allowing it to scale off-screen
        if(CheckCollisionPointRec({ScaleButton.x + ScaleButton.width + MouseXYDelta.x, ScaleButton.y + ScaleButton.height + MouseXYDelta.y}, MainWindow))
        {   
            Update(AnchorX, AnchorY, LenX + MouseXYDelta.x, LenY + MouseXYDelta.y);
        }
    }
}


int Frames::GetSmallestFrameSide(float SideX, float SideY)
{
    return std::min(SideX, SideY);
}

#include "../include/Frames.h"

Frames::Frames()
{
    EdgeButtonSize = 20;
    ActiveFrame = false;
    IsDragging = false;
    IsScaling = false;
}


void Frames::Update(int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY)
{
    //Update all position and size data when frame is moved or scaled
    FrameArea.width = SetLenX;
    FrameArea.height = SetLenY;
    FrameArea.x = SetAnchorX;
    FrameArea.y = SetAnchorY;

    MoveButton.height = MoveButton.width = EdgeButtonSize;
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

    Vector2 MouseXYDelta = GetMouseDelta();

    if (IsDragging) //The click occurs on the move button
    {
        //Stops it from being dragged off-screen, but feels kind of clunky?
        
        if(CheckCollisionRecs(FrameArea, MainWindow))
        {
            Update(MouseXY.x - EdgeButtonSize/2, MouseXY.y - EdgeButtonSize/2, FrameArea.width, FrameArea.height);
        }
        else
        {
            IsDragging= false;
        }

    }
    else if (IsScaling)  //The click occurs on the scale button
    {
        //Not allowing it to scale off-screen
        if(CheckCollisionPointRec({ScaleButton.x + ScaleButton.width + MouseXYDelta.x, ScaleButton.y + ScaleButton.height + MouseXYDelta.y}, MainWindow))
        {   
            Update(FrameArea.x, FrameArea.y, FrameArea.width + MouseXYDelta.x, FrameArea.height + MouseXYDelta.y);
        }
    }
}


int Frames::GetSmallestFrameSide(float SideX, float SideY)
{
    return std::min(SideX, SideY);
}

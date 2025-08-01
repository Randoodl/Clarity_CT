#include "../include/ToolContainer.h"


ToolContainer::ToolContainer()
{
    //Initialise Frame, Dial and Square for the Colour picker
    ColourPicker.Update(20, 20, 400, 400);
    RGBDial.Update(ColourPicker.AnchorX + ColourPicker.LenX/2, ColourPicker.AnchorY + ColourPicker.LenY/2, ColourPicker.LenX/2);

    SelectedShadeFrame.Update(500, 20, 300, 300);

    FrameIsMutable = false;
}


void ToolContainer::DrawElements()
{
    //Simply combining all drawing calls
    RGBDial.DrawRGBDial();
    DrawRectangle(600, 20, 200, 200, RGBDial.CurrentShadeColour);


    if(FrameIsMutable)
    {
        ColourPicker.DrawFrameBox();
        SelectedShadeFrame.DrawFrameBox();
    }
}


void ToolContainer::LeftMouseClickHandler()
{
    //Left Mouse Button
    Vector2 MouseXY = GetMousePosition();

    if(CheckCollisionPointRec(MouseXY, ColourPicker.FrameArea))
    {   
        //Currently clicking within the RGBDial Frame
        InteractWithRGBDial(MouseXY);
    }
    else if (CheckCollisionPointRec(MouseXY, SelectedShadeFrame.FrameArea))
    {
        //Currently clicking within the Shade preview square
        InteractWithShadedSquare(MouseXY);
    }
    
}


void ToolContainer::InteractWithRGBDial(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {   
        //Mouse clicks are meant to deal with the RGBDial itself 
        RGBDial.UpdateRGBSquareColour(MouseXY);
    }
    else
    {
        //Mouse clicks are meant to move and scale the Frame
        ColourPicker.AdjustFrame(MouseXY, 1.0);
        RGBDial.Update(ColourPicker.AnchorX + ColourPicker.LenX/2, ColourPicker.AnchorY + ColourPicker.LenY/2, ColourPicker.LenX/2);
    }
}


void ToolContainer::InteractWithShadedSquare(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {   
        //Mouse clicks are meant to deal with the RGBDial itself 
        //SelectedShadeFrame.UpdateRGBSquareColour(MouseXY);
    }
    else
    {
        //Mouse clicks are meant to move and scale the Frame
        SelectedShadeFrame.AdjustFrame(MouseXY, 1.0);
        //RGBDial.Update(ColourPicker.AnchorX + ColourPicker.LenX/2, ColourPicker.AnchorY + ColourPicker.LenY/2, ColourPicker.LenX/2);
    }
}
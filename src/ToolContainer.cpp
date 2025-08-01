#include "../include/ToolContainer.h"
#include <iostream>

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

    //TEMPORARY FRAME TO SHOW SHADES
    //MOVE THIS TO OWN CLASS - PERHAPS SOME KIND OF PALETTE PICKER
    DrawRectangle(SelectedShadeFrame.AnchorX, SelectedShadeFrame.AnchorY, SelectedShadeFrame.LenX, SelectedShadeFrame.LenY, RGBDial.CurrentShadeColour);

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

        //This now links back to the shade square, updating it to reflect the new Hue selected from the dial
        RGBDial.CurrentShadeColour = RGBDial.RGBSquare.GetSquareRGB(RGBDial.RGBSquare.CurrentShadeMouseLocation); 
    }
    else
    {
        //Mouse clicks are meant to move and scale the Frame
        ColourPicker.AdjustFrame(MouseXY);
        int SmallestFrameSide = ColourPicker.GetSmallestFrameSide(ColourPicker.LenX/2, ColourPicker.LenY/2);  //This ensures the dial is sized to the smallest side of the frame
        RGBDial.Update(ColourPicker.AnchorX + ColourPicker.LenX/2, ColourPicker.AnchorY + ColourPicker.LenY/2, SmallestFrameSide);
    }
}


void ToolContainer::InteractWithShadedSquare(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {   
        //Mouse clicks are meant to deal with the Shade Square itself 
        //SelectedShadeFrame.UpdateRGBSquareColour(MouseXY);
        std::cout << "(" << int(RGBDial.CurrentShadeColour.r) << ", " << int(RGBDial.CurrentShadeColour.g) << ", " << int(RGBDial.CurrentShadeColour.b) << ")\n";
    }
    else
    {
        //Mouse clicks are meant to move and scale the Frame
        SelectedShadeFrame.AdjustFrame(MouseXY);
        //RGBDial.Update(ColourPicker.AnchorX + ColourPicker.LenX/2, ColourPicker.AnchorY + ColourPicker.LenY/2, ColourPicker.LenX/2);
    }
}
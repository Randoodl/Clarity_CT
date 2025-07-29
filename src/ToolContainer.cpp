#include "../include/ToolContainer.h"


ToolContainer::ToolContainer()
{
    //Initialise Frame, Dial and Square for the Colour picker
    ColourPicker.Update(20, 20, 250, 250);
    RGBDial.Update(ColourPicker.AnchorX + ColourPicker.LenX/2, ColourPicker.AnchorY + ColourPicker.LenY/2, ColourPicker.LenX/2);
    FrameIsMutable = false;
}


void ToolContainer::DrawElements()
{
    //Simply combining all drawing calls
    RGBDial.DrawRGBDial();

    if(FrameIsMutable)
    {
        ColourPicker.DrawFrameBox();
    }
}


void ToolContainer::LeftMouseClickHandler()
{
    //Left Mouse Button
    Vector2 MouseXY = GetMousePosition();

    if((MouseXY.x > ColourPicker.AnchorX && MouseXY.x < (ColourPicker.LenX + ColourPicker.AnchorX)) &&  //This could potentially be a point-in-rec check
        (MouseXY.y > ColourPicker.AnchorY && MouseXY.y < (ColourPicker.LenY + ColourPicker.AnchorY)))   //Though that would require an extra, perhaps otherwise useless, Rectangle
        {   
            //Currently clicking within the RGBDial Frame
            InteractWithRGBDial(MouseXY);
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
        Vector2 MouseXYDelta = GetMouseDelta();
        //Mouse clicks are meant to move and scale the Frame
        if (CheckCollisionPointRec(MouseXY, ColourPicker.MoveButton))
        {   
            //The click occurs on the move button
            ColourPicker.Update(MouseXYDelta.x + ColourPicker.AnchorX, MouseXYDelta.y + ColourPicker.AnchorY, ColourPicker.LenX , ColourPicker.LenY);
        }
        else if (CheckCollisionPointRec(MouseXY, ColourPicker.ScaleButton))
        {
            //The click occurs on the scale button
            ColourPicker.Update(ColourPicker.AnchorX, ColourPicker.AnchorY, ColourPicker.LenX + MouseXYDelta.x, ColourPicker.LenY + MouseXYDelta.y);
            
            ColourPicker.SetFrameRatio(1.0); //Rework this, it is a silly way to force a ratio
        }

        RGBDial.Update(ColourPicker.AnchorX + ColourPicker.LenX/2, ColourPicker.AnchorY + ColourPicker.LenY/2, ColourPicker.LenX/2);
    }
}
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

    if((MouseXY.x > ColourPicker.AnchorX && MouseXY.x < (ColourPicker.LenX + ColourPicker.AnchorX)) &&
        (MouseXY.y > ColourPicker.AnchorY && MouseXY.y < (ColourPicker.LenY + ColourPicker.AnchorY)))
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
        //Mouse clicks are meant to move and scale the Frame
        /*
        NOTE: There's GOT to be a better way to check if mouse clicks fall into an area like this
        */
        if ((MouseXY.x > ColourPicker.MoveButtonRoot[0] && MouseXY.x < (ColourPicker.MoveButtonRoot[0] + ColourPicker.EdgeButtonSize)) &&
            (MouseXY.y > ColourPicker.MoveButtonRoot[1] && MouseXY.y < (ColourPicker.MoveButtonRoot[1] + ColourPicker.EdgeButtonSize)))
        {   
            //The click occurs on the move button
            //This updates the Frame's origin to MouseXY and snaps the center of the button to the mouse so it doesn't get locked in the corner
            ColourPicker.Update(MouseXY.x - ColourPicker.EdgeButtonSize/2 , MouseXY.y - ColourPicker.EdgeButtonSize/2, 
                                    ColourPicker.LenX, ColourPicker.LenY);
            
        }
        else if ((MouseXY.x > ColourPicker.ScaleButtonRoot[0] && MouseXY.x < ColourPicker.ScaleButtonRoot[0] + ColourPicker.EdgeButtonSize) &&
                 (MouseXY.y > ColourPicker.ScaleButtonRoot[1] && MouseXY.y < ColourPicker.ScaleButtonRoot[1] + ColourPicker.EdgeButtonSize))
        {
            //The click occurs on the scale button
            //Some cursed relative math fuckery going on here, will probably need to revisit this later
            ColourPicker.Update(ColourPicker.AnchorX, ColourPicker.AnchorY, 
                                    (ColourPicker.LenX + MouseXY.x - ColourPicker.LenX - ColourPicker.AnchorX + ColourPicker.EdgeButtonSize/2) , 
                                    (ColourPicker.LenY + MouseXY.y - ColourPicker.LenY - ColourPicker.AnchorY + ColourPicker.EdgeButtonSize/2));
            
            ColourPicker.SetFrameRatio(1.0); //Rework this, it is a silly way to force a ratio
        }
        
        RGBDial.Update(ColourPicker.AnchorX + ColourPicker.LenX/2, ColourPicker.AnchorY + ColourPicker.LenY/2, ColourPicker.LenX/2);
    }
}
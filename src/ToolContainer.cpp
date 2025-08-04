#include "../include/ToolContainer.h"
#include <iostream>

ToolContainer::ToolContainer()
{
    //Initialise Frame, Dial and Square for the Colour picker
    RGBDialFrame.Update(20, 20, 400, 400);
    RGBDial.Update(RGBDialFrame.AnchorX + RGBDialFrame.LenX/2, RGBDialFrame.AnchorY + RGBDialFrame.LenY/2, RGBDialFrame.LenX/2);

    SelectedShadeFrame.Update(500, 20, 300, 300);
    FrameIsMutable = false;
    SetAllInterActionsToFalse(); //This used to set FrameIsMutable to False!
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
        RGBDialFrame.DrawFrameBox();
        SelectedShadeFrame.DrawFrameBox();
    }
}


void ToolContainer::MouseClickHandler()
{
    //Clunky, hopefully temporary, way to handle continuous single press inputs
    //As more elements get added, this will become way too chonky

    Vector2 MouseXY = GetMousePosition();

    //Each frame checks if a mouse button is pressed, on the frame that it does, it might set an interactible bool to true
    //While the mouse is held down, this IsMousePressed evaluates to false, but the interactible bool remains true until the mouse button is released
    if(IsMouseButtonPressed(0))
    {
        SetElementInteraction(MouseXY);
    }
    
    //As soon as a frame passes wherein the mouse button is released, all interactible bools are set to false
    if(IsMouseButtonReleased(0))
    {
        SetAllInterActionsToFalse();
    }
    //I believe only one Element can be true at a time, unless elements are stacked on top of each other and multiple point-rec checks evaluate to true
    //Since IsMousePressed should only evaluate true for one frame
    InteractWithElement(MouseXY);

    //DEBUGDEBUGDEBUG
    if(IsMouseButtonPressed(2)){if(FrameIsMutable){FrameIsMutable = false;}else{FrameIsMutable=true;}}
    //DEBUGDEBUGDEBUG
}

/*
InteractWithElement, SetElementInteraction and SetFrameInteraction look really damn clunky
At any point, only one if-statement within the function can be set to true
this feels like it should be a switch statement, if only I had a better brain
*/

void ToolContainer::InteractWithElement(Vector2 MouseXY)
{
    if(CursorOnDial){InteractWithRGBDial(MouseXY);}
    if(CursorOnShadeSquare){InteractWithShadedSquare(MouseXY);}
}


void ToolContainer::SetElementInteraction(Vector2 MouseXY)
{
    if(CheckCollisionPointRec(MouseXY, RGBDialFrame.FrameArea)){CursorOnDial = true;}
    if(CheckCollisionPointRec(MouseXY, SelectedShadeFrame.FrameArea)){CursorOnShadeSquare = true;}
}


void ToolContainer::SetFrameInteraction(Vector2 MouseXY)
{
    if(CheckCollisionPointRec(MouseXY, RGBDialFrame.MoveButton)){RGBDialFrame.IsDragging = true;}
    if(CheckCollisionPointRec(MouseXY, RGBDialFrame.ScaleButton)){RGBDialFrame.IsScaling = true;}  
    if(CheckCollisionPointRec(MouseXY, SelectedShadeFrame.MoveButton)){SelectedShadeFrame.IsDragging = true;}
    if(CheckCollisionPointRec(MouseXY, SelectedShadeFrame.ScaleButton)){SelectedShadeFrame.IsScaling = true;}
}


void ToolContainer::SetAllInterActionsToFalse()
{
    //This feels like such a bandaid solution until I can wrap my head around these event handlers properly
    CursorOnDial= false;
    CursorOnShadeSquare = false;
    RGBDialFrame.IsDragging = false;
    RGBDialFrame.IsScaling = false;
    SelectedShadeFrame.IsScaling = false;
    SelectedShadeFrame.IsDragging = false;
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
        SetFrameInteraction(MouseXY);
        RGBDialFrame.AdjustFrame(MouseXY);
        int SmallestFrameSide = RGBDialFrame.GetSmallestFrameSide(RGBDialFrame.LenX/2, RGBDialFrame.LenY/2);  //This ensures the dial is sized to the smallest side of the frame
        RGBDial.Update(RGBDialFrame.AnchorX + RGBDialFrame.LenX/2, RGBDialFrame.AnchorY + RGBDialFrame.LenY/2, SmallestFrameSide);
    }
}


void ToolContainer::InteractWithShadedSquare(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {   
        //Mouse clicks are meant to deal with the Shade Square itself 
        std::cout << "(" << int(RGBDial.CurrentShadeColour.r) << ", " << int(RGBDial.CurrentShadeColour.g) << ", " << int(RGBDial.CurrentShadeColour.b) << ")\n";
    }
    else
    {
        //Mouse clicks are meant to move and scale the Frame
        SetFrameInteraction(MouseXY);
        SelectedShadeFrame.AdjustFrame(MouseXY);
    }
}
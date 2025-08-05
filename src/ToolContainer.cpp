#include "../include/ToolContainer.h"
#include <iostream>

ToolContainer::ToolContainer()
{
    //Initialise the Colour Dial's Frame and Element
    RGBDialFrame.Update(20, 20, 400, 400);
    RGBDial.Update(RGBDialFrame.FrameArea.x + RGBDialFrame.FrameArea.width/2, 
                   RGBDialFrame.FrameArea.y + RGBDialFrame.FrameArea.height/2, 
                   RGBDialFrame.FrameArea.width/2);

    //Initialise the Square's Frame and Element
    DialOffsets = RGBDial.GetSquareInDialOffsets();
    RGBSquareFrame.Update(DialOffsets.x, DialOffsets.y, DialOffsets.z, DialOffsets.z);
    RGBSquare.Update(RGBSquareFrame.FrameArea);

    //Initialise the currently selected Shade from the ShadeSquare
    SelectedShadeFrame.Update(500, 20, 300, 300);
    CurrentShadeColour = {255, 0, 0, 255};
    
    FrameIsMutable = false;

    ElementFrames = {&RGBSquareFrame, &RGBDialFrame, &SelectedShadeFrame};

    SetAllInterActionsToFalse(); //This used to set FrameIsMutable to False!
}


void ToolContainer::DrawElements()
{
    //Simply combining all drawing calls
    RGBDial.DrawRGBDial();
    RGBSquare.DrawShadeSquare();

    //TEMPORARY FRAME TO SHOW SHADES
    //MOVE THIS TO OWN CLASS - PERHAPS SOME KIND OF PALETTE PICKER
    DrawRectangle(SelectedShadeFrame.FrameArea.x, SelectedShadeFrame.FrameArea.y, 
                  SelectedShadeFrame.FrameArea.width, SelectedShadeFrame.FrameArea.height, CurrentShadeColour);

    if(FrameIsMutable)
    {
        RGBDialFrame.DrawFrameBox();
        SelectedShadeFrame.DrawFrameBox();
    }
}


void ToolContainer::MouseClickHandler()
{
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
    DecideElementInteraction(MouseXY);

    //DEBUGDEBUGDEBUG
    if(IsMouseButtonPressed(2)){if(FrameIsMutable){FrameIsMutable = false;}else{FrameIsMutable=true;}}
    //DEBUGDEBUGDEBUG
}


void ToolContainer::DecideElementInteraction(Vector2 MouseXY)
{
    //Still a wee bit clunky, but gets the job done for now
    if(RGBDialFrame.ActiveFrame){InteractWithRGBDial(MouseXY);}
    if(RGBSquareFrame.ActiveFrame){InteractWithShadeSquare(MouseXY);}
    if(SelectedShadeFrame.ActiveFrame){InteractWithShadePreview(MouseXY);}

}


void ToolContainer::SetElementInteraction(Vector2 MouseXY)
{
    //Cycle through each element currently loaded and compare its Frame area to the CLICKED cursor, sets the Frame of the clicked element to true
    //Also enables Frame draggign and scaling, which might cause issues down the line
    //Since it'll be set as Frame: True, Button: True
    for(Frames* Frame: ElementFrames)
    {
        if(CheckCollisionPointRec(MouseXY, Frame->FrameArea))
        {
            Frame->ActiveFrame = true;

            if(FrameIsMutable) //Perhaps we're supposed to interact with the frame and not the element itself
            {
                if(CheckCollisionPointRec(MouseXY, Frame->MoveButton))
                {
                    Frame->IsDragging = true;
                }
                if(CheckCollisionPointRec(MouseXY, Frame->ScaleButton))
                {
                    Frame->IsScaling = true;
                }
            }
            return;
        }
    }
}


void ToolContainer::SetAllInterActionsToFalse()
{
    //Forces all interactions to false as soon as a mouse-up is detected
    for(Frames* Frame: ElementFrames)
    {
        Frame->ActiveFrame = false;
        Frame->IsDragging = false;
        Frame->IsScaling = false;
    }
}


void ToolContainer::InteractWithRGBDial(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {   
        //Mouse clicks are meant to deal with the RGBDial itself 

        //Get the base saturate colour for the RGB square and draw a small indicator bubble
        RGBSquare.SquareBaseColour = RGBDial.GetSaturateColour(MouseXY);
        RGBSquare.ConvertVectorToTexture(RGBSquare.GetVectorOfPixels());

        //This now links back to the shade square, updating it to reflect the new Hue selected from the dial
        CurrentShadeColour = RGBSquare.GetSquareRGB(RGBSquare.CurrentShadeMouseLocation); 
    }
    else
    {   
        //Mouse clicks are meant to move and scale the Frame
        //First, adjust the frame
        RGBDialFrame.AdjustFrame(MouseXY);

        //Then, adjust the RGBDial
        int SmallestFrameSide = RGBDialFrame.GetSmallestFrameSide(RGBDialFrame.FrameArea.width/2, 
                                                                  RGBDialFrame.FrameArea.height/2);  //This ensures the dial is sized to the smallest side of the frame
        
        RGBDial.Update(RGBDialFrame.FrameArea.x + RGBDialFrame.FrameArea.width/2,
                       RGBDialFrame.FrameArea.y + RGBDialFrame.FrameArea.height/2, 
                       SmallestFrameSide);
        
        //Lastly, the RGBSquareFrame is relative to the dial, update that one too
        DialOffsets = RGBDial.GetSquareInDialOffsets();
        RGBSquareFrame.Update(DialOffsets.x, DialOffsets.y, DialOffsets.z, DialOffsets.z);
        RGBSquare.Update(RGBSquareFrame.FrameArea);
    }
}


void ToolContainer::InteractWithShadeSquare(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {
        if(CheckCollisionPointRec(MouseXY, RGBSquareFrame.FrameArea))  //Ensure the cursor can't add MouseXY values outside of the given frame
        {
            CurrentShadeColour = RGBSquare.GetSquareRGB(MouseXY);
        }
    }
}


void ToolContainer::InteractWithShadePreview(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {   
        //Mouse clicks are meant to deal with the Shade Square itself 
        std::cout << "(" << int(CurrentShadeColour.r) << ", " << int(CurrentShadeColour.g) << ", " << int(CurrentShadeColour.b) << ")\n";
    }
    else
    {
        //Mouse clicks are meant to move and scale the Frame
        SelectedShadeFrame.AdjustFrame(MouseXY);
    }
}
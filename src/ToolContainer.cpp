#include "../include/ToolContainer.h"
#include <iostream>

ToolContainer::ToolContainer()
{
    FrameIsMutable = false;
    DialOffsets = {0, 0, 0};
    CurrentShadeColour = {255, 0, 0, 255};
    ElementFrames = {&RGBSquareFrame, &RGBDialFrame, &SelectedShadeFrame, &ToolBarFrame};

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
    SelectedShadeFrame.Update(500, 200, 150, 150);
    
    //Toolbar for various utilities
    ToolBarFrame.Update(700, 600, 210, 70);
    Tools.Update(ToolBarFrame.FrameArea);
    SetAllInterActionsToFalse();
}


void ToolContainer::DrawElements()
{
    //Simply combining all drawing calls
    RGBDial.DrawRGBDial();
    RGBSquare.DrawShadeSquare();
    Tools.DrawToolBar();  //This has to be the last draw call, it has to ALWAYS be accessible

    //TEMPORARY FRAME TO SHOW SHADES
    //MOVE THIS TO OWN CLASS - PERHAPS SOME KIND OF PALETTE PICKER
    DrawRectangle(SelectedShadeFrame.FrameArea.x, SelectedShadeFrame.FrameArea.y, 
                  SelectedShadeFrame.FrameArea.width, SelectedShadeFrame.FrameArea.height, CurrentShadeColour);

    if(FrameIsMutable)
    {
        RGBDialFrame.DrawFrameBox();
        SelectedShadeFrame.DrawFrameBox();
        ToolBarFrame.DrawFrameBox();
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
}


void ToolContainer::DecideElementInteraction(Vector2 MouseXY)
{
    //Still a wee bit clunky, but gets the job done for now
    if(RGBDialFrame.ActiveFrame){InteractWithRGBDial(MouseXY);}
    if(RGBSquareFrame.ActiveFrame){InteractWithShadeSquare(MouseXY);}
    if(SelectedShadeFrame.ActiveFrame){InteractWithShadePreview(MouseXY);}
    if(ToolBarFrame.ActiveFrame){InteractWithToolBar(MouseXY);}

}


void ToolContainer::SetElementInteraction(Vector2 MouseXY)
{
    //Cycle through each element currently loaded and compare its Frame area to the CLICKED cursor, sets the Frame of the clicked element to true
    //Also enables Frame dragging and scaling, which might cause issues down the line
    //Since it'll be set as Frame: True, Button: True
    for(Frames* Frame: ElementFrames)
    {
        if(CheckCollisionPointRec(MouseXY, Frame->FrameArea))
        {
            Frame->ActiveFrame = true;

            Frame->MouseOffsetX = Frame->FrameArea.x - MouseXY.x;
            Frame->MouseOffsetY = Frame->FrameArea.y - MouseXY.y;

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
    //Reset all frame interactions as soon as a mouse-up is detected
    for(Frames* Frame: ElementFrames)
    {
        Frame->ActiveFrame = false;
        Frame->IsDragging = false;
        Frame->IsScaling = false;
        Frame->MouseOffsetX = 0;        
        Frame->MouseOffsetY = 0;        
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

        //So that the ShadeViewBox updates alongside, we need a relative location of the PreviewerXY to the ShadeSquare
        float RelativeDistanceX = float(RGBSquare.CurrentShadeMouseLocation.x - RGBSquareFrame.FrameArea.x) / float(RGBSquareFrame.FrameArea.width);
        float RelativeDistanceY = float(RGBSquare.CurrentShadeMouseLocation.y - RGBSquareFrame.FrameArea.y) / float(RGBSquareFrame.FrameArea.height);

        //Then, adjust the frame
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

        //Preserve the realtive locations between the ShadeSquare and ShadeViewBox
        RGBSquare.CurrentShadeMouseLocation.x = RGBSquareFrame.FrameArea.x + float(RGBSquareFrame.FrameArea.width * RelativeDistanceX);
        RGBSquare.CurrentShadeMouseLocation.y = RGBSquareFrame.FrameArea.y + float(RGBSquareFrame.FrameArea.height * RelativeDistanceY);

        RGBSquare.Update(RGBSquareFrame.FrameArea);
    }
}


void ToolContainer::InteractWithShadeSquare(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {
        if(RGBSquareFrame.ActiveFrame)  //Ensure the cursor can't add MouseXY values outside of the given frame
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


void ToolContainer::InteractWithToolBar(Vector2 MouseXY)
{
    //This is going to be a fun one, because both the Lock and Reset functions
    //need to be accessible irrespective of the FrameIsMutable state

    if(!FrameIsMutable)
    {
        if(CheckCollisionPointRec(MouseXY, Tools.SaveButton))
        {
             std::cout << "Save\n";
        }   
    }
    else
    {   
        ToolBarFrame.AdjustFrame(MouseXY);
        Tools.Update(ToolBarFrame.FrameArea);
    }

    if(!ToolBarFrame.IsDragging && !ToolBarFrame.IsScaling) //This essentially stops a click-through when using the Adjustment buttons on the frame
    {
        if(CheckCollisionPointRec(MouseXY, Tools.LockButton))
        {
            if(FrameIsMutable){FrameIsMutable = false;}else{FrameIsMutable=true;} //Simple bool swap statement
            ToolBarFrame.ActiveFrame = false; //stops a held down click from spamming the button
        }
        if(CheckCollisionPointRec(MouseXY, Tools.ResetButton))
        {
             std::cout << "Reset\n";
        }
    }
}
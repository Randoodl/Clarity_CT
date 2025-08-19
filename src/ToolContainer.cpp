#include "../include/ToolContainer.h"
#include <iostream>

ToolContainer::ToolContainer()
{   
    //Initialise all colours in the collection.
    ColourCollection.BaseHueColour = {255, 0, 0, 255};
    ColourCollection.ShadedColour = ColourCollection.BaseHueColour;
    ColourCollection.UpdateComplement();

    FrameIsMutable = false;
    DialOffsets = {0, 0, 0};
    ElementFrames = {&ToolBarFrame, &RGBSquareFrame, &RGBDialFrame, &MainShadesTintsFrame};
    VisibleFrames = {&RGBDialFrame, &ToolBarFrame, &MainShadesTintsFrame};

    //Initialise the Colour Dial's Frame and Element
    RGBDialFrame.Update(0, 0, 400, 400);
    RGBDial.Update(RGBDialFrame.FrameArea.x + RGBDialFrame.FrameArea.width/2, 
                   RGBDialFrame.FrameArea.y + RGBDialFrame.FrameArea.height/2, 
                   RGBDialFrame.FrameArea.width/2);

    //Initialise the Square's Frame and Element
    DialOffsets = RGBDial.GetSquareInDialOffsets();
    RGBSquareFrame.Update(DialOffsets.x, DialOffsets.y, DialOffsets.z, DialOffsets.z);
    RGBSquare.Update(RGBSquareFrame.FrameArea);

    //Initialise Frame and Element for the main colour's Shades and Tints
    MainShadesTintsFrame.Update(410, 20, 600, 50);
    MainShadesTints.Update(MainShadesTintsFrame.FrameArea, 9, 20);
    MainShadesTints.GenerateShadesTints(ColourCollection.ShadedColour);
    MainShadesTints.GeneratePaletteRectangles();

    //Toolbar for various utilities
    ToolBarFrame.Update(700, 600, 280, 70);
    Tools.Update(ToolBarFrame.FrameArea);
    SetAllInterActionsToFalse();
    UpdateWindowMinimumSize();
}


void ToolContainer::DrawElements()
{
    //Simply combining all drawing calls
    RGBDial.DrawRGBDial();
    RGBSquare.DrawShadeSquare();
    MainShadesTints.DrawPalette();
    Tools.DrawToolBar();  //This has to be the last draw call, it has to ALWAYS be accessible

    if(FrameIsMutable)
    {
        for(Frames* ShowFrame : VisibleFrames){ShowFrame->DrawFrameBox();}
    }
    //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
    DrawRectangle(1300, 100, 70, 70, ColourCollection.BaseHueColour);
    DrawRectangle(1370, 100, 70, 70, ColourCollection.ShadedColour);
    DrawRectangle(1300, 170, 70, 70, ColourCollection.ComplementColour);
    DrawRectangle(1370, 170, 70, 70, ColourCollection.ShadedComplementColour);
    //DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG
}


void ToolContainer::SnapFrames()
{
    //Size all frames down to snugly wrap the element they contain after frame mutability is toggled

    //Snap Frame to ToolBar
    ToolBarFrame.Update(ToolBarFrame.FrameArea.x, ToolBarFrame.FrameArea.y, Tools.ButtonContainer.width, Tools.ButtonContainer.height);

    //Snap Frame to RGBDial
    RGBDialFrame.Update(RGBDial.DialOriginXY.x - RGBDial.DialOuterRadius, RGBDial.DialOriginXY.y - RGBDial.DialOuterRadius, 
                        RGBDial.DialOuterRadius * 2, RGBDial.DialOuterRadius * 2);

    UpdateWindowMinimumSize();
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
    if(ToolBarFrame.ActiveFrame){InteractWithToolBar(MouseXY); return;}
    if(RGBSquareFrame.ActiveFrame){InteractWithShadeSquare(MouseXY); return;}
    if(RGBDialFrame.ActiveFrame){InteractWithRGBDial(MouseXY); return;}
    if(MainShadesTintsFrame.ActiveFrame){InteractWithMainShadesTints(MouseXY); return;}
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
        ColourCollection.BaseHueColour = RGBSquare.SquareBaseColour;
        ColourCollection.UpdateComplement();
        RGBSquare.ConvertVectorToTexture(RGBSquare.GetVectorOfPixels());

        //This now links back to the shade square, updating it to reflect the new Hue selected from the dial
        ColourCollection.ShadedColour = RGBSquare.GetSquareRGB(RGBSquare.CurrentShadeMouseLocation, ColourCollection.BaseHueColour); 
        MainShadesTints.GenerateShadesTints(ColourCollection.ShadedColour);
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
            //Calculate the main hue colour shade
            ColourCollection.ShadedColour = RGBSquare.GetSquareRGB(MouseXY, ColourCollection.BaseHueColour);
            RGBSquare.ShadedColour = ColourCollection.ShadedColour;

            //Calculate the Complement colour shade
            ColourCollection.ShadedComplementColour = RGBSquare.GetSquareRGB(MouseXY, ColourCollection.ComplementColour);

            MainShadesTints.GenerateShadesTints(ColourCollection.ShadedColour);
        }
    }
}


void ToolContainer::InteractWithMainShadesTints(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {
        Color SelectedColour = MainShadesTints.GetVariationColour(MouseXY);
        std::cout << "(" << int(SelectedColour.r) << ", " << int(SelectedColour.g) << ", " << int(SelectedColour.b) << ")\n"; 
    }
    else
    {
        MainShadesTintsFrame.AdjustFrame(MouseXY);
        MainShadesTints.Update(MainShadesTintsFrame.FrameArea, MainShadesTints.VariationAmount, MainShadesTints.VariationDelta);
        MainShadesTints.GeneratePaletteRectangles();
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
        if(CheckCollisionPointRec(MouseXY, Tools.ColourModeButton))
        {
            std::cout << "Colour\n";
        }

        ToolBarFrame.ActiveFrame = false; //stops a held down click from spamming the button   
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
            if(FrameIsMutable)
            {
                FrameIsMutable = false;
                SnapFrames();
            }
            else
            {
                FrameIsMutable=true;
            }
        }

        if(CheckCollisionPointRec(MouseXY, Tools.ResetButton))
        {
            std::cout << "Reset\n";
            
        }

        ToolBarFrame.ActiveFrame = false; //stops a held down click from spamming the button
    }
}


void ToolContainer::UpdateWindowMinimumSize()
{
    //Make sure the window can't size down to exclude parts of, or entire Frames
    int MinWidth {0};
    int MinHeight  {0};

    //Keep trawling through Frames' bottomright points and retain the largest coordinate point
    for(Frames* Frame : ElementFrames)
    {
        if((Frame->FrameArea.x + Frame->FrameArea.width) > MinWidth)
        {
            MinWidth = (Frame->FrameArea.x + Frame->FrameArea.width);
        }
        if((Frame->FrameArea.y + Frame->FrameArea.height) > MinHeight)
        {
            MinHeight = (Frame->FrameArea.y + Frame->FrameArea.height);
        }
    }

    //Then set the max coordinate point as the min window size
    SetWindowMinSize(MinWidth, MinHeight);
}


void ToolContainer::UnloadAllFonts()
{
    UnloadFont(MainShadesTints.SetFont);
}
#include "../include/ToolContainer.h"
#include <iostream>

ToolContainer::ToolContainer()
{
    FrameIsMutable = false;
    DialOffsets = {0, 0, 0};
    CurrentShadeColour = {255, 0, 0, 255};
    ElementFrames = {&ToolBarFrame, &RGBSquareFrame, &RGBDialFrame, &ShadesTintsFrame, &ComplementsFrame};
    VisibleFrames = {&RGBDialFrame, &ShadesTintsFrame, &ComplementsFrame, &ToolBarFrame};

    //Initialise the Colour Dial's Frame and Element
    RGBDialFrame.Update(0, 0, 400, 400);
    RGBDial.Update(RGBDialFrame.FrameArea.x + RGBDialFrame.FrameArea.width/2, 
                   RGBDialFrame.FrameArea.y + RGBDialFrame.FrameArea.height/2, 
                   RGBDialFrame.FrameArea.width/2);

    //Initialise the Square's Frame and Element
    DialOffsets = RGBDial.GetSquareInDialOffsets();
    RGBSquareFrame.Update(DialOffsets.x, DialOffsets.y, DialOffsets.z, DialOffsets.z);
    RGBSquare.Update(RGBSquareFrame.FrameArea);

    //Initialise the shades and tints of currently selected colour
    ShadesTintsFrame.Update(0, 415, 400, 70);
    ShadesTintsAmount = 10; 
    ShadesTintsStep = 15;   
    CombinedShadesTintsUpdate(true, CurrentShadeColour, ShadesTintsFrame, ShadesFrame, Shades, TintsFrame, Tints);  //Initialises Frames and Palettes for Shades and Tints, passes True because FrameIsMutable is not set to True yet here

    //Initialise the complement of the current colour, plus its shades and tints
    ComplementsFrame.Update(0, 500, 400, 70);
    ComplementColour = {0, 255, 255, 255};
    CombinedShadesTintsUpdate(true, ComplementColour, ComplementsFrame, ComplementShadesFrame, ComplementShades, ComplementTintsFrame, ComplementTints);


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
    Shades.DrawPalette();
    Tints.DrawPalette();
    ComplementShades.DrawPalette();
    ComplementTints.DrawPalette();
    Tools.DrawToolBar();  //This has to be the last draw call, it has to ALWAYS be accessible

    if(FrameIsMutable)
    {
        for(Frames* ShowFrame : VisibleFrames){ShowFrame->DrawFrameBox();}
    }
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
    if(ShadesTintsFrame.ActiveFrame){InteractWithShadesAndTints(MouseXY); return;}
    if(ComplementsFrame.ActiveFrame){InteractWithComplementShadesAndTints (MouseXY); return;}
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
        ComplementColour = GetComplementColour(CurrentShadeColour);
        CombinedShadesTintsUpdate(FrameIsMutable, CurrentShadeColour, ShadesTintsFrame, ShadesFrame, Shades, TintsFrame, Tints);
        CombinedShadesTintsUpdate(FrameIsMutable, ComplementColour, ComplementsFrame, ComplementShadesFrame, ComplementShades, ComplementTintsFrame, ComplementTints);
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
            ComplementColour = GetComplementColour(CurrentShadeColour);
            CombinedShadesTintsUpdate(FrameIsMutable, CurrentShadeColour, ShadesTintsFrame, ShadesFrame, Shades, TintsFrame, Tints);
            CombinedShadesTintsUpdate(FrameIsMutable, ComplementColour, ComplementsFrame, ComplementShadesFrame, ComplementShades, ComplementTintsFrame, ComplementTints);
        }
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
            ToolBarFrame.ActiveFrame = false; //stops a held down click from spamming the button
        }
        if(CheckCollisionPointRec(MouseXY, Tools.ResetButton))
        {
             std::cout << "Reset\n";
        }
    }
}


void ToolContainer::InteractWithShadesAndTints(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {   
        Color SelectedColour {};

        if(CheckCollisionPointRec(MouseXY, ShadesFrame.FrameArea))
        {
            SelectedColour = Shades.GetVariationColour(MouseXY);
        }
        else if(CheckCollisionPointRec(MouseXY, TintsFrame.FrameArea))
        {
            SelectedColour = Tints.GetVariationColour(MouseXY);
        }

        std::cout << "(" << int(SelectedColour.r) << ", " << int(SelectedColour.g) << ", " << int(SelectedColour.b) << ")\n"; 
    }
    else
    {
        ShadesTintsFrame.AdjustFrame(MouseXY);
        CombinedShadesTintsUpdate(FrameIsMutable, CurrentShadeColour, ShadesTintsFrame, ShadesFrame, Shades, TintsFrame, Tints);
    }
}


void ToolContainer::InteractWithComplementShadesAndTints(Vector2 MouseXY)
{
    if(!FrameIsMutable)
    {   
        Color SelectedColour {};

        if(CheckCollisionPointRec(MouseXY, ComplementShadesFrame.FrameArea))
        {
            SelectedColour = ComplementShades.GetVariationColour(MouseXY);
        }
        else if(CheckCollisionPointRec(MouseXY, ComplementTintsFrame.FrameArea))
        {
            SelectedColour = ComplementTints.GetVariationColour(MouseXY);
        }

        std::cout << "(" << int(SelectedColour.r) << ", " << int(SelectedColour.g) << ", " << int(SelectedColour.b) << ")\n"; 
    }
    else
    {
        ComplementsFrame.AdjustFrame(MouseXY);
        CombinedShadesTintsUpdate(FrameIsMutable, ComplementColour, ComplementsFrame, ComplementShadesFrame, ComplementShades, ComplementTintsFrame, ComplementTints);
    }
}


void ToolContainer::CombinedShadesTintsUpdate(bool FrameHasChanged, Color &SeedColour, Frames &ContainingFrame, Frames &FrameOfShades, Palette &PaletteOfShades, Frames &FrameOfTints,  Palette &PaletteOfTints)
{
    //Since this gets called multiple times, might as well do a combined single call
    
    if(FrameHasChanged) //Only need to invoke moving/scaling logic if the frame has been touched
    {
        //Decide whether Palette cascades horizontally or vertically
        bool CascadeHorizontally = ContainingFrame.FrameArea.width >= ContainingFrame.FrameArea.height;  
        bool CascadeVertically   = ContainingFrame.FrameArea.width <  ContainingFrame.FrameArea.height;

        //This looks complex, but really just boils down to toggling "half the Frames' width" or "Offset the Frames' x" based on the above bools
        FrameOfShades.Update(ContainingFrame.FrameArea.x, ContainingFrame.FrameArea.y, 
                      ContainingFrame.FrameArea.width / (1 + CascadeVertically), ContainingFrame.FrameArea.height / (1 + CascadeHorizontally));
        FrameOfTints.Update(ContainingFrame.FrameArea.x + (CascadeVertically * (ContainingFrame.FrameArea.width / (1 + CascadeVertically))), 
                          ContainingFrame.FrameArea.y + (CascadeHorizontally * (ContainingFrame.FrameArea.height / (1 + CascadeHorizontally))), 
                          ContainingFrame.FrameArea.width / (1 + CascadeVertically),
                          ContainingFrame.FrameArea.height/ (1 + CascadeHorizontally));

        //Update Palette relative to frame
        PaletteOfShades.Update(FrameOfShades.FrameArea, ShadesTintsAmount);
        PaletteOfTints.Update(FrameOfTints.FrameArea, ShadesTintsAmount);
    }

    //At any rate, recalculate the square positions and colours
    PaletteOfShades.UpdateShadesTints(SeedColour, true, ShadesTintsStep, RGBValMin);
    PaletteOfTints.UpdateShadesTints(SeedColour, false, ShadesTintsStep, RGBValMax);
}


Color ToolContainer::GetComplementColour(Color SeedColour)
{
    //Get the complement of the current shade colour
    Color ReturnColour = SeedColour;
    ReturnColour.r = RGBValMax - SeedColour.r;  
    ReturnColour.g = RGBValMax - SeedColour.g;
    ReturnColour.b = RGBValMax - SeedColour.b;

    return ReturnColour;
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
    UnloadFont(Shades.SetFont);
    UnloadFont(Tints.SetFont);
}
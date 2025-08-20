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

    //This keeps looping every frame, but essentially snags the first and ONLY true active frame
    for(int IndexOfFrame {0}; IndexOfFrame < int(ElementFrames.size()); ++IndexOfFrame)
    {
        if(ElementFrames[IndexOfFrame]->ActiveFrame)
        {
            DecideElementInteraction(MouseXY, IndexOfFrame);
        }
    }
}


void ToolContainer::DecideElementInteraction(Vector2 MouseXY, int ActiveElementFrame)
{
    switch(ActiveElementFrame)
    {
        case 0: 
            ElementInteractions::InteractWithToolBar(MouseXY, FrameIsMutable, ToolBarFrame, Tools);
            if(!FrameIsMutable){SnapFrames();} //This does get called every time a button is pressed, not terrible but not great?
            break;
        case 1:
            ElementInteractions::InteractWithShadeSquare(MouseXY, FrameIsMutable, RGBSquareFrame, RGBSquare, MainShadesTints, ColourCollection);
            break;
        case 2:
            ElementInteractions::InteractwithRGBDial(MouseXY,FrameIsMutable, RGBSquareFrame, RGBDialFrame, RGBSquare, RGBDial, MainShadesTints, ColourCollection, DialOffsets); 
            break;
        case 3:
            ElementInteractions::InteractWithMainShadesTints(MouseXY, FrameIsMutable, MainShadesTintsFrame, MainShadesTints);
            break;
        default:
            break;
    }
}


void ToolContainer::SetElementInteraction(Vector2 MouseXY)
{
    //Cycle through each element currently loaded and compare its Frame area to the CLICKED cursor, sets the Frame of the clicked element to true

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
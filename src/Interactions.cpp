#include "../include/Interactions.h"
#include <iostream>

ElementInteractions::ElementInteractions(bool& PassedFrameState, ColourFamily& PassedColourFamily, std::vector<Palette*>& PassedPalettes, std::map<Palette*, std::vector<Color*>>& PassedPaletteActions) : 
                                         R_FrameState(PassedFrameState), R_ColourFamily(PassedColourFamily), R_AllPalettes(PassedPalettes), R_PaletteActions(PassedPaletteActions)
{
    PassedMouseXY = {0, 0};
}

void ElementInteractions::InteractWithToolBar(Frames& ToolBarFrame, ToolBar& Tools, Color& SetBackGroundColour, Color& SetToolBarBackgroundColour, Color& SetButtonColour)
{
    //This is going to be a fun one, because both the Lock and Reset functions
    //need to be accessible irrespective of the FrameIsMutable state

    if(!R_FrameState)
    {
        if(CheckCollisionPointRec(PassedMouseXY, Tools.SaveButton))
        {
            std::cout << "Save\n";
        }   
        if(CheckCollisionPointRec(PassedMouseXY, Tools.OptionsButton))
        {
            for(auto ChangeColour : {&SetBackGroundColour, &SetToolBarBackgroundColour, &SetButtonColour})
            {
                if(ChangeColour->r < RGBValMax/2)
                {
                    ChangeColour->r = ChangeColour->r + (0.6 * RGBValMax); 
                    ChangeColour->g = ChangeColour->g + (0.6 * RGBValMax);
                    ChangeColour->b = ChangeColour->b + (0.6 * RGBValMax); 
                }
                else
                {
                    ChangeColour->r = ChangeColour->r - (0.6 * RGBValMax); 
                    ChangeColour->g = ChangeColour->g - (0.6 * RGBValMax);
                    ChangeColour->b = ChangeColour->b - (0.6 * RGBValMax); 
                }
            }
        }

        ToolBarFrame.ActiveFrame = false; //stops a held down click from spamming the button   
    }
    else
    {   
        ToolBarFrame.AdjustFrame(PassedMouseXY);
        Tools.Update(ToolBarFrame.FrameArea);
    }

    if(!ToolBarFrame.IsDragging && !ToolBarFrame.IsScaling) //This essentially stops a click-through when using the Adjustment buttons on the frame
    {
        if(CheckCollisionPointRec(PassedMouseXY, Tools.LockButton))
        {
            if(R_FrameState){R_FrameState = false;}else{R_FrameState = true;}  
        }
        if(CheckCollisionPointRec(PassedMouseXY, Tools.ResetButton))
        {
            std::cout << "Reset\n";
        }

        ToolBarFrame.ActiveFrame = false; //stops a held down click from spamming the button
    }
}


void ElementInteractions::InteractWithShadeSquare(Frames& RGBSquareFrame, ShadeSquare& RGBSquare)
{
    if(!R_FrameState)
    {
        if(RGBSquareFrame.ActiveFrame)  //Ensure the cursor can't add MouseXY values outside of the given frame
        {
            //Calculate the main hue colour shade
            R_ColourFamily.ShadedColour = RGBSquare.GetSquareRGB(PassedMouseXY, R_ColourFamily.BaseHueColour);
            RGBSquare.ShadedColour = R_ColourFamily.ShadedColour;

            //Calculate the Complement colour shade
            R_ColourFamily.ShadedComplementColour = RGBSquare.GetSquareRGB(PassedMouseXY, R_ColourFamily.ComplementColour);

            //Set updated complement and triad colours
            R_ColourFamily.Update();

            //Update Palettes
            UpdatePaletteColours(R_PaletteActions);
        }
    }
}


void ElementInteractions::InteractwithRGBDial(Frames& RGBSquareFrame, Frames& RGBDialFrame, ShadeSquare& RGBSquare, ColourDial& RGBDial, Vector3& DialOffsets)
{
    if(!R_FrameState)
    {
        //Get the base saturate colour for the RGB square and draw a small indicator bubble
        RGBSquare.SquareBaseColour = RGBDial.GetSaturateColour(PassedMouseXY);
        
        //Update the ColourCollection based on the new Hue
        R_ColourFamily.BaseHueColour = RGBSquare.SquareBaseColour;
        R_ColourFamily.Update();
        
        //Generate the RGBSquare for the selected Hue
        RGBSquare.ConvertVectorToTexture(RGBSquare.GetVectorOfPixels());

        //Link changes made in the SquareRGB back to the Shade version of colours in ColorCollection
        //So that when the dial is updated, the Shaded colours are updated alongside
        //ColourCollection needs to be updated again to reflect changes in the Shade
        R_ColourFamily.ShadedColour = RGBSquare.GetSquareRGB(RGBSquare.CurrentShadeMouseLocation, R_ColourFamily.BaseHueColour);
        R_ColourFamily.ShadedComplementColour = RGBSquare.GetSquareRGB(RGBSquare.CurrentShadeMouseLocation, R_ColourFamily.ComplementColour);
        R_ColourFamily.Update();
        
        //Update Palettes
        UpdatePaletteColours(R_PaletteActions);
    }
    else
    {
        //So that the ShadeViewBox updates alongside, we need a relative location of the ShadeViewBoxXY to the ShadeSquareXY
        float RelativeDistanceX = float(RGBSquare.CurrentShadeMouseLocation.x - RGBSquareFrame.FrameArea.x) / float(RGBSquareFrame.FrameArea.width);
        float RelativeDistanceY = float(RGBSquare.CurrentShadeMouseLocation.y - RGBSquareFrame.FrameArea.y) / float(RGBSquareFrame.FrameArea.height);

        //Then, adjust the frame
        RGBDialFrame.AdjustFrame(PassedMouseXY);

        //Then, adjust the RGBDial, making sure to scale it relative to the smallest side
        int SmallestFrameSide = RGBDialFrame.GetSmallestFrameSide(RGBDialFrame.FrameArea.width/2,
                                                                  RGBDialFrame.FrameArea.height/2);  

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


void ElementInteractions::InteractWithPalette(Frames& PaletteFrame, Palette& PaletteColours)
{
    if(!R_FrameState)
    {
        Color ChosenColour = PaletteColours.GetVariationColour(PassedMouseXY);
        if(ChosenColour.a != 0) //Don't update if the alpha value is zero, which only happens if MouseXY is outside of the coloured Palette rects
        {
            R_ColourFamily.CurrentSelectedColour = ChosenColour;
        } 
    }
    else
    {
        PaletteFrame.AdjustFrame(PassedMouseXY);
        PaletteColours.Update(PaletteFrame.FrameArea, PaletteColours.VariationAmount, PaletteColours.VariationDelta);
        PaletteColours.GeneratePaletteRectangles();
    }
}


void ElementInteractions::UpdatePaletteColours(std::map<Palette*, std::vector<Color*>>& PassedPaletteActions)
{
    //This method simplifies the updating of Palette colours when RGBDial or ShadeSquare is interacted with

    for(auto const& [PaletteKey, ColourVector] : PassedPaletteActions)
    {
        //Go through all entries of the Palette-ColourVector map
        
        if(ColourVector.size() == 2) //These are colour previews
        {
            PaletteKey->SetHueShadePair(*ColourVector[0], *ColourVector[1]);
        }
        else //These are ShadesTints
        {
            PaletteKey->GenerateShadesTints(*ColourVector[0]);
        }
    }
}


void ElementInteractions::InteractWithFloodFilledFrame(Frames& FloodedFrame, Color& FillColour)
{
    if(!R_FrameState)
    {
        std::cout << "(" << int(FillColour.r) << ", " << int(FillColour.g) << ", " << int(FillColour.b) << ")\n"; 
    }
    else
    {
        FloodedFrame.AdjustFrame(PassedMouseXY);
    }
}






#include "../include/Interactions.h"

#include <iostream>


ElementInteractions::ElementInteractions()
{
    PassedMouseXY = {0, 0};
    PassedFrameState = false; 
}

void ElementInteractions::InteractWithToolBar(Frames& ToolBarFrame, ToolBar& Tools)
{
    //This is going to be a fun one, because both the Lock and Reset functions
    //need to be accessible irrespective of the FrameIsMutable state

    if(!PassedFrameState)
    {
        if(CheckCollisionPointRec(PassedMouseXY, Tools.SaveButton))
        {
            std::cout << "Save\n";
        }   
        if(CheckCollisionPointRec(PassedMouseXY, Tools.OptionsButton))
        {
            std::cout << "Colour\n";
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
            if(PassedFrameState){PassedFrameState = false;}else{PassedFrameState = true;}  
        }
        if(CheckCollisionPointRec(PassedMouseXY, Tools.ResetButton))
        {
            std::cout << "Reset\n";
        }

        ToolBarFrame.ActiveFrame = false; //stops a held down click from spamming the button
    }
}


void ElementInteractions::InteractWithShadeSquare(Frames& RGBSquareFrame, ShadeSquare& RGBSquare, 
                                                  Palette& MainShadesTints, Palette& ComplementShadesTints, ColourFamily& ColourCollection)
{
    if(!PassedFrameState)
    {
        if(RGBSquareFrame.ActiveFrame)  //Ensure the cursor can't add MouseXY values outside of the given frame
        {
            //Calculate the main hue colour shade
            ColourCollection.ShadedColour = RGBSquare.GetSquareRGB(PassedMouseXY, ColourCollection.BaseHueColour);
            RGBSquare.ShadedColour = ColourCollection.ShadedColour;

            //Calculate the Complement colour shade
            ColourCollection.ShadedComplementColour = RGBSquare.GetSquareRGB(PassedMouseXY, ColourCollection.ComplementColour);

            //Update Palettes
            MainShadesTints.GenerateShadesTints(ColourCollection.ShadedColour);
            ComplementShadesTints.GenerateShadesTints(ColourCollection.ShadedComplementColour);
        }
    }
}


void ElementInteractions::InteractwithRGBDial(Frames& RGBSquareFrame, Frames& RGBDialFrame, ShadeSquare& RGBSquare,
                                              ColourDial& RGBDial, Palette& MainShadesTints, Palette& ComplementShadesTints, ColourFamily& ColourCollection, Vector3& DialOffsets)
{
    if(!PassedFrameState)
    {
        //Get the base saturate colour for the RGB square and draw a small indicator bubble
        RGBSquare.SquareBaseColour = RGBDial.GetSaturateColour(PassedMouseXY);
        
        //Update the ColourCollection based on the new Hue
        ColourCollection.BaseHueColour = RGBSquare.SquareBaseColour;
        ColourCollection.SetComplement(ColourCollection.BaseHueColour, ColourCollection.ComplementColour);

        //Generate the RGBSquare for the selected Hue
        RGBSquare.ConvertVectorToTexture(RGBSquare.GetVectorOfPixels());

        //Link changes made in the SquareRGB back to the Shade version of colours in ColorCollection
        //So that when the dial is updated, the Shaded colours are updated alongside
        ColourCollection.ShadedColour = RGBSquare.GetSquareRGB(RGBSquare.CurrentShadeMouseLocation, ColourCollection.BaseHueColour);
        ColourCollection.ShadedComplementColour = RGBSquare.GetSquareRGB(RGBSquare.CurrentShadeMouseLocation, ColourCollection.ComplementColour);
        
        //Update Palettes
        MainShadesTints.GenerateShadesTints(ColourCollection.ShadedColour);
        ComplementShadesTints.GenerateShadesTints(ColourCollection.ShadedComplementColour);
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


void ElementInteractions::InteractWithShadesTints(Frames& ShadesTintsFrame, Palette& ShadesTints)
{
    if(!PassedFrameState)
    {
        Color SelectedColour = ShadesTints.GetVariationColour(PassedMouseXY);
        if(SelectedColour.a != 0)
        {
            std::cout << "(" << int(SelectedColour.r) << ", " << int(SelectedColour.g) << ", " << int(SelectedColour.b) << ")\n"; 
        }
    }
    else
    {
        ShadesTintsFrame.AdjustFrame(PassedMouseXY);
        ShadesTints.Update(ShadesTintsFrame.FrameArea, ShadesTints.VariationAmount, ShadesTints.VariationDelta);
        ShadesTints.GeneratePaletteRectangles();
    }
}


void ElementInteractions::InteractWithFloodFilledFrame(Frames& FloodedFrame, Color& FillColour)
{
    if(!PassedFrameState)
    {
        std::cout << "(" << int(FillColour.r) << ", " << int(FillColour.g) << ", " << int(FillColour.b) << ")\n"; 
    }
    else
    {
        FloodedFrame.AdjustFrame(PassedMouseXY);
    }
}






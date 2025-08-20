#include "../include/Interactions.h"

#include <iostream>


void ElementInteractions::InteractWithToolBar(Vector2 MouseXY, bool& FrameIsMutable, Frames& ToolBarFrame, ToolBar& Tools)
{
    //This is going to be a fun one, because both the Lock and Reset functions
    //need to be accessible irrespective of the FrameIsMutable state

    if(!FrameIsMutable)
    {
        if(CheckCollisionPointRec(MouseXY, Tools.SaveButton))
        {
            std::cout << "Save\n";
        }   
        if(CheckCollisionPointRec(MouseXY, Tools.OptionsButton))
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
            if(FrameIsMutable){FrameIsMutable = false;}else{FrameIsMutable=true;}
        }
        if(CheckCollisionPointRec(MouseXY, Tools.ResetButton))
        {
            std::cout << "Reset\n";
            
        }

        ToolBarFrame.ActiveFrame = false; //stops a held down click from spamming the button
    }
}


void ElementInteractions::InteractWithShadeSquare(Vector2 MouseXY, bool FrameIsMutable, Frames& RGBSquareFrame, ShadeSquare& RGBSquare, 
                                                  Palette& MainShadesTints, ColourFamily& ColourCollection)
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


void ElementInteractions::InteractwithRGBDial(Vector2 MouseXY, bool FrameIsMutable, Frames& RGBSquareFrame, Frames& RGBDialFrame, ShadeSquare& RGBSquare,
                                              ColourDial& RGBDial, Palette& MainShadesTints, ColourFamily& ColourCollection, Vector3& DialOffsets)
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


void ElementInteractions::InteractWithMainShadesTints(Vector2 MouseXY, bool FrameIsMutable, Frames& MainShadesTintsFrame, Palette& MainShadesTints)
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






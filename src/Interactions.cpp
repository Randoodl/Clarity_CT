#include "../include/Interactions.h"


ElementInteractions::ElementInteractions(bool& PassedFrameState, ColourFamily& PassedColourFamily, std::vector<Palette*>& PassedPalettes, std::map<Palette*, std::vector<Color*>>& PassedPaletteActions) : 
                                         R_FrameState(PassedFrameState), R_ColourFamily(PassedColourFamily), R_AllPalettes(PassedPalettes), R_PaletteActions(PassedPaletteActions)
{
    PassedMouseXY = {0, 0};
    ResetFrames = false;
}

void ElementInteractions::InteractWithToolBar(std::vector<Frames*>& PassedFrames, ToolBar& Tools, bool& DarkModeEnabled, bool& HexModeEnabled, char*& PassedBinPath)
{
    //This is going to be a fun one, because both the Lock and Reset functions
    //need to be accessible irrespective of the FrameIsMutable state

    if(!R_FrameState)
    {
        if(CheckCollisionPointRec(PassedMouseXY, Tools.SaveButton))
        {
            //Seems like a long goddamn way to pass down main(argv), but alas, here we are
            //I'm tired, Samwise
            ExportElementPositions(PassedFrames, DarkModeEnabled, HexModeEnabled,  PassedBinPath);
            std::cout << "Exported config file\n";
        }   
        if(CheckCollisionPointRec(PassedMouseXY, Tools.CodeModeButton))
        {
            //Toggle HexMode from 0 to 1 or vice versa
            if(HexModeEnabled){HexModeEnabled = false;}else{HexModeEnabled = true;};
        }
        if(CheckCollisionPointRec(PassedMouseXY, Tools.DarkModeButton))
        {
            //Toggle DarkMode from 0 to 1 or vice versa
            if(DarkModeEnabled){DarkModeEnabled = false;}else{DarkModeEnabled = true;};
        }

        //Not best practice to access ToolBarFrame by indexing into the vector, but if we keep it locked at the front 
        //of the vector (as well it should), eh? Maybe ok?
        PassedFrames[0]->ActiveFrame = false; //stops a held down click from spamming the button   
    }
    else
    {   
        PassedFrames[0]->AdjustFrame(PassedMouseXY);
        Tools.Update(PassedFrames[0]->FrameArea);
    }

    if(!PassedFrames[0]->IsDragging && !PassedFrames[0]->IsScaling) //This essentially stops a click-through when using the Adjustment buttons on the frame
    {
        if(CheckCollisionPointRec(PassedMouseXY, Tools.LockButton))
        {
            if(R_FrameState){R_FrameState = false;}else{R_FrameState = true;}  
        }
        if(CheckCollisionPointRec(PassedMouseXY, Tools.ResetButton))
        {
            ResetFrames = true;
            std::cout << "Reset config file\n";
        }
        PassedFrames[0]->ActiveFrame = false; //stops a held down click from spamming the button
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


void ElementInteractions::InteractWithFloodFilledFrame(Frames& FloodedFrame, Color& FillColour, bool& PassedCodeMode)
{
    if(!R_FrameState)
    {
        GetRGBValuesToClipboard(int(FillColour.r), int(FillColour.g), int(FillColour.b), PassedCodeMode);  
    }
    else
    {
        FloodedFrame.AdjustFrame(PassedMouseXY);
    }
}


void ElementInteractions::ExportElementPositions(std::vector<Frames*>& PassedFrames, bool PassedDarkMode, bool PassedHexMode, char*& PassedBinPath)
{
    //Export the current Frames x, y, height and width to a local .conf file

    //Get the parent directory of the Clarity_CT executable
    std::filesystem::path ExportPath {PassedBinPath};
    ExportPath = ExportPath.parent_path();

    try
    {
        //Open (or create) the config file
        std::ofstream ExportFile(ExportPath / "Clarity.conf");

        //Store all Frames' positonal data in [x, y, width, height] format
        //It counts up to n of Frames, so the last line for DarkMode toggling does not get counted here
        for(auto Line : PassedFrames) 
        {
            ExportFile << Line->FrameArea.x << "," << Line->FrameArea.y << "," << Line->FrameArea.width << "," << Line->FrameArea.height << "\n";
        }

        //Tack on a Dec/Hex mode at the almost end
        ExportFile << PassedHexMode << "\n";

        //Tack on a Light/Dark mode at the actual end
        ExportFile << PassedDarkMode;

        ExportFile.close();
    }
    catch(...) //Not doing any real error handling here, just if you canny do it, you canny
    {
        std::cout << "Cannot write to config file\n";
    }
}


void ElementInteractions::GetRGBValuesToClipboard(int ValueR, int ValueG, int ValueB, bool& PassedCodeMode)
{
    //Copy the RGB values to the clipboard, either as a (R,G,B) tuple or hexcode

    std::string CopyString;

    if(PassedCodeMode)
    {
        //Hex code

        //Store pieces of 00-ff hex codes in here
        std::stringstream HexValue;

        for(int Value : {ValueR, ValueG, ValueB})
        { 
            if(Value < 16){HexValue << "0";} //Sloppy way to add trailing zeroes, but it works!
            HexValue << std::hex << Value;
        }
        CopyString = HexValue.str();
    }
    else
    {
        //Decimal tuple
        CopyString = std::to_string(ValueR) + "," + 
                     std::to_string(ValueG) + "," + 
                     std::to_string(ValueB);
    }
    SetClipboardText(CopyString.c_str());
}
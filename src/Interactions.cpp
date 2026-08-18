/* 
 * Copyright (c) 2026, Dylan Ooijevaar
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../include/Interactions.h"


ElementInteractions::ElementInteractions(bool& PassedFrameState, ColourFamily& PassedColourFamily, std::map<Palette*, std::vector<Color*>>& PassedPaletteActions) : 
                                         R_FrameState(PassedFrameState), R_ColourFamily(PassedColourFamily), R_PaletteActions(PassedPaletteActions)
{
    PassedMouseXY = {};
    ResetFrames = false;
    ToggleHidden = true;
}


void ElementInteractions::InteractWithToolBar(std::vector<Frames*>& PassedFrames, ToolBar& Tools, bool& DarkModeEnabled, bool& HexModeEnabled, std::string& PassedConfigPath)
{
    if(!R_FrameState)
    {
        if(CheckCollisionPointRec(PassedMouseXY, Tools.ToggleHiddenButton))
        {
            if(ToggleHidden){ToggleHidden = false;}else{ToggleHidden = true;};
        }
        else if(CheckCollisionPointRec(PassedMouseXY, Tools.SaveButton))
        {
            //Seems like a long goddamn way to pass down main(argv), but alas, here we are
            //I'm tired, Samwise
            //  This was already an ass-backwards way of storing .conf files, but now that it is stored in ~/.config or %appdata% it's even sillier
            //  The entire labyrinthine pipeline of passing the .conf location here was already built, so I decided to keep it this way.
            //  What's FOSS without a little bit of tech debt, anyway?
            ExportElementPositions(PassedFrames, DarkModeEnabled, HexModeEnabled,  PassedConfigPath);
            std::cout << "Exported config file.\n";
        } 
        else if(CheckCollisionPointRec(PassedMouseXY, Tools.CodeModeButton))
        {
            //Toggle HexMode from 0 to 1 or vice versa
            if(HexModeEnabled){HexModeEnabled = false;}else{HexModeEnabled = true;};
        }
        else if(CheckCollisionPointRec(PassedMouseXY, Tools.DarkModeButton))
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
            std::cout << "Adopted default UI settings.\n";
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
        RGBSquareFrame.Update(DialOffsets.x, DialOffsets.y, DialOffsets.z, DialOffsets.z, RGBSquareFrame.ShowContent, RGBSquareFrame.FrameTag);

        //Preserve the relative locations between the ShadeSquare and ShadeViewBox
        RGBSquare.CurrentShadeMouseLocation.x = RGBSquareFrame.FrameArea.x + float(RGBSquareFrame.FrameArea.width * RelativeDistanceX);
        RGBSquare.CurrentShadeMouseLocation.y = RGBSquareFrame.FrameArea.y + float(RGBSquareFrame.FrameArea.height * RelativeDistanceY);
        RGBSquare.Update(RGBSquareFrame.FrameArea);
    }
}


void ElementInteractions::InteractWithPalette(Frames& PaletteFrame, Palette& PaletteColours)
{
    if(!R_FrameState && PaletteFrame.ShowContent)
    {
        Color ChosenColour = PaletteColours.GetVariationColour(PassedMouseXY);
        if(ChosenColour.a != RGBValMin) 
        {
            //Don't update if the alpha value is zero, which only happens if MouseXY is outside of the coloured Palette rects
            R_ColourFamily.CurrentSelectedColour = ChosenColour;
        } 
    }
    else
    {
        PaletteFrame.AdjustFrame(PassedMouseXY);
        PaletteColours.Update(PaletteFrame.FrameArea, PaletteColours.VariationAmount, PaletteColours.VariationDelta, PaletteFrame.ShowContent);
        PaletteColours.GeneratePaletteRectangles();
    }
}


void ElementInteractions::InteractWithFloodFilledFrame(Frames& FloodedFrame, Color& FillColour, bool& PassedCodeMode, std::vector<Color>& PassedDropperColours)
{
    if(!R_FrameState && FloodedFrame.ShowContent)
    {
        //First, copy the values (either HEX or RGB)
        GetRGBValuesToClipboard(int(FillColour.r), int(FillColour.g), int(FillColour.b), PassedCodeMode);  

        //Then, if the colour is not being tracked yet, do so
        if(!CompareColours(PassedDropperColours, FillColour))
        {
            //Stored colours follow a FIFO principle
            PassedDropperColours.pop_back();
            PassedDropperColours.insert(PassedDropperColours.begin(), FillColour);
        }

        //Stop the button from being spammed when held down
        FloodedFrame.ActiveFrame = false;
    }
    else
    {
        FloodedFrame.AdjustFrame(PassedMouseXY);
    }
}


void ElementInteractions::InteractWithColourDropper(Frames& ColourDropperFrame, ColourDropper& RGBDropper)
{
    if(!R_FrameState && ColourDropperFrame.ShowContent)
    {
        Color ChosenColour = RGBDropper.GetDropperValue(PassedMouseXY);
        if(ChosenColour.a != RGBValMin) 
        {
            //Don't update if the alpha value is zero, which happens when clicking on an empty cell or if the function returns a dummy BLANK
            R_ColourFamily.CurrentSelectedColour = ChosenColour;
        }

        //Stop the button from being spammed when held down
        ColourDropperFrame.ActiveFrame = false;
    }
    else
    {
        ColourDropperFrame.AdjustFrame(PassedMouseXY);
        RGBDropper.Update(ColourDropperFrame.FrameArea, ColourDropperFrame.ShowContent);
        RGBDropper.GenerateDropperRectangles();
    }
}


void ElementInteractions::ExportElementPositions(std::vector<Frames*>& PassedFrames, bool PassedDarkMode, bool PassedHexMode, std::string& PassedConfigPath)
{
    //Export the current Frames [x, y, width, height] , DarkMode and CodeMode to a local .conf file
    try
    {
        //Open (or create) the config file
        std::ofstream ExportFile(PassedConfigPath + "/Clarity.conf");

        //Store all Frames' positonal data in [x, y, width, height] format
        //It counts up to n of Frames, so the last lines for DarkMode/HexMode toggling do not get counted here
        for(auto Line : PassedFrames) 
        { 
            ExportFile << Line->FrameTag << ", "<< Line->FrameArea.x << "," << Line->FrameArea.y << "," 
            << Line->FrameArea.width << "," << Line->FrameArea.height << "," << Line->ShowContent <<"\n";
        }

        //Tack on a Dec/Hex mode at the almost end
        ExportFile << PassedHexMode << "\n";

        //Tack on a Light/Dark mode at the actual end
        ExportFile << PassedDarkMode;

        ExportFile.close();
    }
    catch(...) //Not doing any real error handling here, just if you canny do it, you canny
    {
        std::cout << "Cannot write to config file.\n";
    }
}


void ElementInteractions::GetRGBValuesToClipboard(int ValueR, int ValueG, int ValueB, bool& PassedCodeMode)
{
    //Copy the RGB values to the clipboard, either as a decimal or hexedecimal values, based on CodeMode

    //Build the string to copy to the clipboard in here
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
        CopyString = '#' + HexValue.str();
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


bool ElementInteractions::CompareColours(std::vector<Color>& KnownColours, Color& FillColour)
{
    //A somewhat hamfisted way to figure out whether or not a colour is already being tracked or not
    bool ColourIsKnown = false;

    for(Color KnownColour : KnownColours)
    {
        if(KnownColour.r == FillColour.r && KnownColour.g == FillColour.g && KnownColour.b == FillColour.b)
        {
            //This is an exact match of RGB to an already stored RGB value
            ColourIsKnown = true;
            break;
        }
    }
    return ColourIsKnown;
}
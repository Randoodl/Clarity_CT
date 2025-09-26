#pragma once

#include <functional>
#include <fstream>
#include <filesystem>
#include <iostream>

#include "./ColourDial.h"
#include "./ShadeSquare.h"
#include "./Frames.h"
#include "./Palette.h"
#include "./ToolBar.h"
#include "./ColourFamily.h"


class ElementInteractions
{
    //This is an attempt to isolate all interactions in their own file with the intent to declutter ToolContainer
    //The arguments passed to these functions follows the following order:
    //
    //  Frames
    //  Single Elements
    //  Palettes
    //  Additional arguments
    //
    //Multiple Frames or Elements are passed in order of descending priority (i.e. ToolBarFrame is always passed as a first frame, if present)

    public:
        //Instead of passing specific attributes like one Color out of the ColourCollection or the FrameState, just load
        //a collection of references along with the ElementInterations instance
        ElementInteractions(bool& PassedFrameState, ColourFamily& PassedColourFamily, std::map<Palette*, std::vector<Color*>>& PassedPaletteActions);
        
        Vector2 PassedMouseXY;

        //The toolbar changes the state of FrameIsMutable, hence we need to work with the actual value and not a copy
        bool& R_FrameState;    
        bool ResetFrames; 
    
        
        //Relevant collections of data passed by reference
        ColourFamily& R_ColourFamily;    
        std::map<Palette*, std::vector<Color*>>& R_PaletteActions;
        
        //All interactions as listed in ToolContainer::DecideElementInteraction
        void InteractWithToolBar(std::vector<Frames*>& PassedFrames, ToolBar& Tools, bool& DarkModeEnabled, bool& HexModeEnabled, char*& PassedBinPath);
        void InteractWithShadeSquare(Frames& RGBSquareFrame, ShadeSquare& RGBSquare);
        void InteractwithRGBDial(Frames& RGBSquareFrame, Frames& RGBDialFrame, ShadeSquare& RGBSquare, ColourDial& RGBDial, Vector3& DialOffsets);
        void InteractWithPalette(Frames& PaletteFrame, Palette& PaletteColours);
        void InteractWithFloodFilledFrame(Frames& FloodedFrame, Color& FillColour, bool& PassedCodeMode);

        //config file exporting functionality tied to ToolBar
        void ExportElementPositions(std::vector<Frames*>& PassedFrames, bool PassedDarkMode, bool PassedHexMode, char*& PassedBinPath);

    private:
        //Relevant to InterActWithFloodFilledFrame
        void GetRGBValuesToClipboard(int ValueR, int ValueG, int ValueB, bool& PassedCodeMode);

        //Relevant to InteractWithRGBDial and InteractWithShadeSquare
        void UpdatePaletteColours(std::map<Palette*, std::vector<Color*>>& PassedPaletteActions);
};
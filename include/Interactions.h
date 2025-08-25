#pragma once

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
        ElementInteractions(bool& PassedFrameState, ColourFamily& PassedColourFamily, std::vector<Palette*>& PassedPalettes, 
                            std::map<Palette*, std::vector<Color*>>& PassedPaletteActions);

        Vector2 PassedMouseXY;

        bool& R_FrameState;              //The toolbar changes the state of FrameIsMutable, hence we need to work with the actual value and not a copy
        ColourFamily& R_ColourFamily;    //Ditto for the ColourCollection
        std::vector<Palette*>& R_AllPalettes;          //And all available Palettes - PERHAPS OBSOLETE BUT KEEPING IT AROUND FOR NOW IN CASE IT COMES IN HANDY LATER
        std::map<Palette*, std::vector<Color*>>& R_PaletteActions;

        void InteractWithToolBar(Frames& ToolBarFrame, ToolBar& Tools);

        void InteractWithShadeSquare(Frames& RGBSquareFrame, ShadeSquare& RGBSquare);

        void InteractwithRGBDial(Frames& RGBSquareFrame, Frames& RGBDialFrame, ShadeSquare& RGBSquare, ColourDial& RGBDial, Vector3& DialOffsets);

        void InteractWithPalette(Frames& PaletteFrame, Palette& PaletteColours);
        void UpdatePaletteColours(std::map<Palette*, std::vector<Color*>>& PassedPaletteActions);

        void InteractWithFloodFilledFrame(Frames& FloodedFrame, Color& FillColour);
};
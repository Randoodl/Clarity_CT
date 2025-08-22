#pragma once

#include "./Interactions.h"

class ToolContainer
{
    //This class is meant to hold all of the program's "moving parts"
    //As well as the methods to interact with 'em
    
    public:
        ToolContainer();

        bool FrameIsMutable;  //This toggles whether or not you can move/scale frames

        void DrawElements();
        void MouseClickHandler();

        void UnloadAllFonts();

    private:  
        void SetElementInteraction(Vector2 MouseXY);
        void SetAllInterActionsToFalse();

        void DecideElementInteraction(int ActiveElementFrame);

        void SnapFrames();
        void UpdateWindowMinimumSize();

        //The collection of relevant colours
        ColourFamily ColourCollection;
        
        //Interactions of elements with the Mouse
        ElementInteractions Interactions = ElementInteractions(FrameIsMutable, ColourCollection);

        //Previews of colours
        Frames SelectedColourFrame;
        Frames BaseHueFrame;
        Frames ComplementFrame;

        //Colour picker
        Frames RGBDialFrame;
        ColourDial RGBDial; 

        //The Shade square within the RGBDial
        Vector3 DialOffsets;
        Frames RGBSquareFrame;
        ShadeSquare RGBSquare;

        //Shades and tints of the currently selected colour
        Frames MainShadesTintsFrame;
        Palette MainShadesTints;

        //Shades and tints of the complement colour
        Frames ComplementShadesTintsFrame;
        Palette ComplementShadesTints;
        
        //A vector of pointers to all frames so we can for-loop through 'em
        std::vector<Frames*> ElementFrames;
        std::vector<Frames*> HiddenFrames;

        //Toolbar
        Frames ToolBarFrame;
        ToolBar Tools;
};
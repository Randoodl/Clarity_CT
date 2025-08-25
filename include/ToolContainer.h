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

        void InitialiseColourPreview(Palette& PreviewPalette, Frames& PreviewFrame, Color& Base, Color& Shade, int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY);

        //The collection of relevant colours
        ColourFamily ColourCollection;
        
        //Interactions of elements with the Mouse
        ElementInteractions Interactions = ElementInteractions(FrameIsMutable, ColourCollection, AllPalettes, PaletteActions);

        //Previews of colours
        Frames BaseHueFrame;
        Palette Hue;
        Frames ComplementFrame;
        Palette Complement;
        Frames LowerTriadFrame;
        Palette LowerTriad;
        Frames UpperTriadFrame;
        Palette UpperTriad;

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

        //A vector of all Palettes and Map of Palettes for specific interactions
        std::vector<Palette*> AllPalettes;
        std::map<Palette*, std::vector<Color*>> PaletteActions;

        //Toolbar
        Frames ToolBarFrame;
        ToolBar Tools;
};
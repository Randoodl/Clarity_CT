#pragma once

#include <string>

#include "./Interactions.h"
#include "./Defaults.h"

class ToolContainer
{
    //This class is meant to hold all of the program's "moving parts"
    //As well as the methods to interact with 'em
    
    public:
        ToolContainer(char*& PassedBinPath);
        Defaults Layout;
        Defaults BackupLayout; //This is what the Elements fall back to if reading the .conf fails

        bool DarkModeEnabled;
        bool HexModeEnabled;
        bool FrameIsMutable;  //This toggles whether or not you can move/scale frames

        void DrawElements();
        void MouseClickHandler();

        void InitialiseAllElements();
        
        //The collection of relevant colours
        ColourFamily ColourCollection;

    private:  
        void SetElementInteraction(Vector2 MouseXY);
        void SetAllInterActionsToFalse();

        void DecideElementInteraction(int ActiveElementFrame);

        void SnapFrames();
        void UpdateWindowMinimumSize();
        void SetUIColours(bool DarkModeEnabled);

        void InitialiseColourPreview(Palette& PreviewPalette, Frames& PreviewFrame, Color& Base, Color& Shade, ElementPosition& SetLayout);
        void InitialiseShadesTints(Palette& ViewPalette, Frames& ViewFrame, Color& PassColour, int VariationAmount, int VariationDelta, ElementPosition& SetLayout);

        char* BinPath;
        void LoadCustomConfig(char*& PassedBinPath);

        void DefaultFallback();
  
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

        Frames CurrentSelectedColourFrame;

        //Colour picker
        Frames RGBDialFrame;
        ColourDial RGBDial; 

        //The Shade square within the RGBDial
        Vector3 DialOffsets;
        Frames RGBSquareFrame;
        ShadeSquare RGBSquare;

        //Shades and tints
        int SetVariationAmount;
        int SetVariationDelta;

        Frames MainShadesTintsFrame;
        Palette MainShadesTints;
        Frames ComplementShadesTintsFrame;
        Palette ComplementShadesTints;
        Frames LowerTriadShadesTintsFrame;
        Palette LowerTriadShadesTints;
        Frames UpperTriadShadesTintsFrame;
        Palette UpperTriadShadesTints;
        
        //A vector of pointers to all frames so we can for-loop through 'em
        std::vector<Frames*> ElementFrames;
        std::vector<Frames*> HiddenFrames;
        std::vector<ElementPosition*> LayoutPositions;

        //A vector of all Palettes and Map of Palettes for specific interactions
        std::vector<Palette*> AllPalettes;
        std::map<Palette*, std::vector<Color*>> PaletteActions;

        //Toolbar
        Frames ToolBarFrame;
        ToolBar Tools;
};
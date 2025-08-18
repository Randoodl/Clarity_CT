#pragma once

#include "./ColourDial.h"
#include "./ShadeSquare.h"
#include "./Frames.h"
#include "./Palette.h"
#include "./ToolBar.h"

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
        void InteractWithRGBDial(Vector2 MouseXY);
        void InteractWithShadeSquare(Vector2 MouseXY);
        void InteractWithToolBar(Vector2 MouseXY);
        void InteractWithShadesAndTints(Vector2 MouseXY);
        void InteractWithComplementShadesAndTints(Vector2 MouseXY);
        void SetElementInteraction(Vector2 MouseXY);
        void SetAllInterActionsToFalse();

        void DecideElementInteraction(Vector2 MouseXY);

        void SnapFrames();
        void UpdateWindowMinimumSize();
        
        //Colour picker
        Frames RGBDialFrame;
        ColourDial RGBDial; 

        //The Shade square within the RGBDial
        Vector3 DialOffsets;
        Frames RGBSquareFrame;
        ShadeSquare RGBSquare;
        Color CurrentShadeColour;

        //Shades and tints of selected colour
        Frames ShadesTintsFrame;
        int ShadesTintsAmount; //How many 'cells' to display
        int ShadesTintsStep; //How much the RGB values should differ between cells
        Frames ShadesFrame;
        Palette Shades;
        Frames TintsFrame;
        Palette Tints;
        void CombinedShadesTintsUpdate(bool FrameHasChanged, Color &SeedColour, Frames &ContainingFrame,
                                       Frames &FrameOfShades, Palette &PaletteOfShades,
                                       Frames &FrameOfTints,  Palette &PaletteOfTints);  //To make updating the position less of a mouthfull

        //Complement colour tints and shades, borrows some members from ShadesAndTintsFrame
        Frames ComplementsFrame;
        //Frames ComplementColourFrame;
        Color ComplementColour;
        Color GetComplementColour(Color SeedColour);
        Frames ComplementShadesFrame;
        Palette ComplementShades;
        Frames ComplementTintsFrame;
        Palette ComplementTints;


        
        //A vector of pointers to all frames so we can for-loop through 'em
        std::vector<Frames*> ElementFrames;
        std::vector<Frames*> VisibleFrames;

        //Toolbar
        Frames ToolBarFrame;
        ToolBar Tools;

};
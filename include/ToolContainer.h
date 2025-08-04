#pragma once

#include "./ColourDial.h"
#include "./Frames.h"

class ToolContainer
{
    //This class is meant to hold all of the program's "moving parts"
    //As well as the methods to interact with 'em
    
    public:
        ToolContainer();

        bool FrameIsMutable;  //This toggles whether or not you can move/scale frames

        void DrawElements();
        void MouseClickHandler();

    private:  
        bool CursorOnDial;
        bool CursorOnShadeSquare;  

        void InteractWithRGBDial(Vector2 MouseXY);
        void InteractWithShadedSquare(Vector2 MouseXY);

        void SetElementInteraction(Vector2 MouseXY);
        void SetFrameInteraction(Vector2 MouseXY);
        void SetAllInterActionsToFalse();

        void InteractWithElement(Vector2 MouseXY);
        
        //Colour picker
        Frames RGBDialFrame;
        ColourDial RGBDial; //Includes ShadeSquare

        //Preview the selected Shade
        Frames SelectedShadeFrame;
};
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
        void LeftMouseClickHandler();
        void RightMouseClickHandler();  //Not in use yet

        //Colour picker
        Frames ColourPicker;
        ColourDial RGBDial;

    private:    
        void InteractWithRGBDial(Vector2 MouseXY);
        int GetRGBColour(Vector2 MousePositionXY, double DistanceToClick);

};
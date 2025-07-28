#pragma once

#include "./GradientSquare.h"
#include "./Frames.h"

class ToolContainer
{
    //This class is meant to hold all of the program's "moving parts"
    //As well as the methods to interact with 'em
    
    public:
        ToolContainer();

        void DrawElements();
        void LeftMouseClickHandler();
        void RightMouseClickHandler();  //Not in use yet

        //Frame, Dial and Square for the Colour Picker
        Frames ColourPicker;
        ColourDial RGBDial;
        GradientSquare RGBSquare;
        void InteractWithRGBDial(Vector2 MouseXY);
        int GetRGBColour(Vector2 MousePositionXY, double DistanceToClick);

};
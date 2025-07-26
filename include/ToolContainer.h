#pragma once

#include "./GradientSquare.h"

class ToolContainer
{
    //This class is meant to hold all of the program's "moving parts"
    //As well as the methods to interact with 'em
    
    public:
        ToolContainer();
        ColourDial RGBDial;
        GradientSquare RGBSquare;

        //Handling user input
        void LeftMouseClickHandler();
        void UpdateGradientSquare(Vector2 MouseXY);

        //Retrieving colour information
        int GetRGBColour(Vector2 MousePositionXY, double DistanceToClick);
};
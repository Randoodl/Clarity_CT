#pragma once

#include <raylib.h>
#include "./Constants.h"

class ColourFamily
{
    //This class is meant to hold a collection of colours relative to a single base RGB value colour
    
    public:
        ColourFamily();

        Color BaseHueColour;
        Color ShadedColour;
        Color ComplementColour;
        Color ShadedComplementColour;

        void SetComplement(Color Base, Color& Complement);   
};
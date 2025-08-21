#include "../include/ColourFamily.h"

ColourFamily::ColourFamily()
{
    BaseHueColour = {0, 0, 0, 255};
    ShadedColour = {0, 0, 0, 255};
    ComplementColour = {0, 0, 0, 255};
    ShadedComplementColour = {0, 0, 0, 255};
}


void ColourFamily::SetComplement(Color Base, Color& Complement)
{
    //Get a direct complement Hue to the base Hue currently set

    Complement.r = RGBValMax - Base.r; 
    Complement.g = RGBValMax - Base.g; 
    Complement.b = RGBValMax - Base.b; 
}
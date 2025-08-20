#include "../include/ColourFamily.h"

ColourFamily::ColourFamily()
{
    BaseHueColour = {0, 0, 0, 255};
    ShadedColour = {0, 0, 0, 255};
    ComplementColour = {0, 0, 0, 255};
    ShadedComplementColour = {0, 0, 0, 255};
}


void ColourFamily::UpdateComplement()
{
    //Get a direct complement Hue to the base Hue currently set

    ComplementColour.r = RGBValMax - BaseHueColour.r; 
    ComplementColour.g = RGBValMax - BaseHueColour.g; 
    ComplementColour.b = RGBValMax - BaseHueColour.b; 
}
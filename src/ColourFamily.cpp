#include "../include/ColourFamily.h"

ColourFamily::ColourFamily()
{
    BaseHueColour = {0, 0, 0, 255};
    ShadedColour = {0, 0, 0, 255};
    ComplementColour = {0, 0, 0, 255};
    ShadedComplementColour = {0, 255, 255, 255};
}


void ColourFamily::UpdateComplement()
{
    //Get a direct complement hue to the base hue currently set

    ComplementColour.r = RGBValMax - BaseHueColour.r; 
    ComplementColour.g = RGBValMax - BaseHueColour.g; 
    ComplementColour.b = RGBValMax - BaseHueColour.b; 
}
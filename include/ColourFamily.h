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
        Color LowerTriadColour;
        Color UpperTriadColour;
        Color LowerTriadShade;
        Color UpperTriadShade;
        
        void Update();

    private:
        void SetComplement(Color Base, Color& Complement);   
        void SetTriad(Color Base, Color& LowerTriad, Color& UpperTriad);


};
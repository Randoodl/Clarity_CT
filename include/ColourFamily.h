/* 
 * Copyright (c) 2025, Dylan Ooijevaar
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <raylib.h>

#include "./Constants.h"


class ColourFamily
{
    //This class is meant to hold a collection of colours relative to a single base RGB value colour as well as some UI colours
    
    public:
        ColourFamily();

        //UI colours
        Color BackgroundColour;
        Color ToolBackgroundColour;
        Color ToolButtonColour;
        Color ToolIconColour;
        Color FrameBoxColour;

        //Interactible colours
        Color CurrentSelectedColour;
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
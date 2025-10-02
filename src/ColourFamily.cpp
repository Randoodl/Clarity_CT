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

#include "../include/ColourFamily.h"


ColourFamily::ColourFamily()
{
    //UI colours
    BackgroundColour = BLACK;
    ToolBackgroundColour = BLACK;
    ToolButtonColour = BLACK;
    ToolIconColour = BLACK;
    FrameBoxColour = BLACK;

    //Interactible colours
    CurrentSelectedColour = {255, 0, 0, 255};
    BaseHueColour = {255, 0, 0, 255};
    ShadedColour  = {255, 0, 0, 255};
    ComplementColour       = BLACK;
    ShadedComplementColour = {0, 255, 255, 255};
    LowerTriadColour       = BLACK;
    UpperTriadColour       = BLACK;
    LowerTriadShade        = BLACK;
    UpperTriadShade        = BLACK;
}


void ColourFamily::Update()
{   
    //Updating the whole colour family in a single call
    
    //Setting the complements of Hue and Shade
    SetComplement(BaseHueColour, ComplementColour);

    //Setting the triads of Hue and Shade
    SetTriad(BaseHueColour, LowerTriadColour, UpperTriadColour);
    SetTriad(ShadedColour, LowerTriadShade, UpperTriadShade);
}


void ColourFamily::SetComplement(Color Base, Color& Complement)
{
    //Get a direct complement Hue to the base Hue currently set
    Complement.r = RGBValMax - Base.r; 
    Complement.g = RGBValMax - Base.g; 
    Complement.b = RGBValMax - Base.b; 
}


void ColourFamily::SetTriad(Color Base, Color& LowerTriad, Color& UpperTriad)
{
    //Take in a colour value and shuffle the channels around to get its triad

    //Downshift channels
    LowerTriad.r = Base.g;
    LowerTriad.g = Base.b;
    LowerTriad.b = Base.r;

    //Upshift channels
    UpperTriad.r = Base.b;
    UpperTriad.g = Base.r;
    UpperTriad.b = Base.g;
}
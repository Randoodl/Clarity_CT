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

#include <map>

#include "./ShadeSquare.h"
#include "./Constants.h"


class ColourDial
{
    //This class is meant to hold all the information for the Hue selection dial
    
    public:
        ColourDial();

        //Publicly mutable attributes for calculating the dial
        Vector2 DialOriginXY;
        float DialBandThickness;
        int DialInnerRadius;
        int DialOuterRadius;

        //Publicly mutable attributes for drawing the dial
        Color CurrentSaturateColour;
        Color GetSaturateColour(Vector2 MouseXY);

        //This is needed to calculate the anchor point and length for the ShadeSquare
        Vector3 GetSquareInDialOffsets();  

        void Update(int SetOriginX, int SetOriginY, int SetOuterRadius);
        void DrawRGBDial(Color& BackgroundColour);

    private:
        int Current_iRGB;  //The current index in the RGBSaturates map that updates when a colour is selected
        
        //Private attributes and methods to calculate and draw the Dial to the screen
        int BandsAmount;                                        //More or less forced to 1530, 3 values in RGB, 2 states each (rising, falling), between 0-255 totals (6 *255) 1530 possible bands
        std::map<int, std::vector<float>> MapOFDialPositions;   //Vectors of X and Y coordinates to draw lines between, creating a dial
        std::map<int, std::vector<int>> MapOfRGBSaturates;      //All 1530 possible saturated RGB colours for each R,G or B value from 0-255
        std::map<int, std::vector<int>> GenerateRGBTuples();
        std::map<int, std::vector<float>> CalculateDialPositions();
        
        //Turn a click into an index in MapOfRGBSaturates that GetSaturateColour can translate into an actual Color
        int GetRGBColour(Vector2 MouseXY, float DistanceToClick);
        
        //Everything needed to draw the indicator bubble along the Dial
        Vector2 BubbleOriginXY;
        float BubbleRadius;  
        void UpdateBubblePosition();        
};
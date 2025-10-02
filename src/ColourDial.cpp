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

#include "../include/ColourDial.h"


ColourDial::ColourDial() 
{
    //public
    DialOriginXY = {0, 0};
    DialBandThickness = 0;
    DialInnerRadius = 0;
    DialOuterRadius = 0;
    CurrentSaturateColour = {255, 0, 0, 255};
   
    //private
    Current_iRGB = 0;
    BandsAmount = 1530;  
    MapOFDialPositions = {};
    MapOfRGBSaturates = GenerateRGBTuples(); 
    BubbleOriginXY = {0, 0};
    BubbleRadius = 0;
}


Color ColourDial::GetSaturateColour(Vector2 MouseXY)
{
    //Set a base saturate colour if the dial is clicked, from which a shade and tint can be generated

    //Using good ol' pythagoras we can calculate the distance from the centre of the dial to the mouseclick
    float DistanceToClick = sqrt(pow(MouseXY.x - DialOriginXY.x, 2) + pow(MouseXY.y - DialOriginXY.y, 2));

    //Using the remainder of GetRGBColour because the Map is created left-to-right, but the dial is created as a unit circle
    //which increments to 2 PI in a counter-clockwise (right-to-left) fashion
    //Using GetRGBColour directly would have colours and the bubble flipped along the Y-axis 
    Current_iRGB = (BandsAmount - GetRGBColour(MouseXY, DistanceToClick)) % BandsAmount;
        
    CurrentSaturateColour.r = MapOfRGBSaturates[Current_iRGB][2];
    CurrentSaturateColour.g = MapOfRGBSaturates[Current_iRGB][1];
    CurrentSaturateColour.b = MapOfRGBSaturates[Current_iRGB][0];
    
    UpdateBubblePosition();

    return CurrentSaturateColour;
}


Vector3 ColourDial::GetSquareInDialOffsets()
{
    //Essentially, from the centre off the RGB Dial, how much do we offset XY to get a corner coordinate that touches the circle of the inner dial.
    //Think of it as drawing a triangle from the centre of the dial with InnerRadius as the hypo: the straight sides are then the length of half the square
    //It's really just about fitting a square into a circle and approaching said square by turning it into triangles
    //Somewhere out there, my Maths teacher must be chuffed I'm finally applying geometry 
    
    //Placeholder Vec3 to overwrite with actual values
    Vector3 Offsets = {0, 0, 0};

    //We have C in A^2*B^2 = C^2, but we need A or B (they are equivalent)
    double AnchorPointOffset = sqrt(0.5 * pow(DialInnerRadius, 2));
    
    //The length of the square is twice the straight side of the triangle drawn within the dial
    Offsets.z = 2 * AnchorPointOffset;  

    //The offset from the centre of the square to the topleft corner touching the inner dial
    Offsets.x = DialOriginXY.x - AnchorPointOffset;
    Offsets.y = DialOriginXY.y - AnchorPointOffset;

    return Offsets;
}


void ColourDial::Update(int SetOriginX, int SetOriginY, int SetOuterRadius)
{
    DialOriginXY.x = SetOriginX;
    DialOriginXY.y = SetOriginY;
    DialInnerRadius = SetOuterRadius - (SetOuterRadius/4); //Approach the Dial's max size from the Frame holding it
    DialOuterRadius = SetOuterRadius;
    DialBandThickness = (DialOuterRadius - DialInnerRadius) * 0.05; //5% of the Dial's thickness is a wild guess, might update later

    UpdateBubblePosition();
    MapOFDialPositions = CalculateDialPositions();  
}


void ColourDial::DrawRGBDial(Color& BackgroundColour)
{
    //Draw a colour dial based on the data calculated in the MapOfDialPositions

    Color BaseColour {BLACK}; //Placeholder colour to update and draw to screen

    //Draw the circular spectrum
    for(int LineSegment {0}; LineSegment < BandsAmount; ++LineSegment)
    {
        BaseColour.r = MapOfRGBSaturates[LineSegment][2];
        BaseColour.g = MapOfRGBSaturates[LineSegment][1];
        BaseColour.b = MapOfRGBSaturates[LineSegment][0];

        DrawLineEx
        (
            {MapOFDialPositions[LineSegment][0], MapOFDialPositions[LineSegment][2]}, 
            {MapOFDialPositions[LineSegment][1], MapOFDialPositions[LineSegment][3]}, 
            DialBandThickness, BaseColour
        );
    }

    //Draw the colour preview bubble
    DrawCircle(BubbleOriginXY.x, BubbleOriginXY.y, (DialOuterRadius - DialInnerRadius)/2, BackgroundColour);
    DrawCircle(BubbleOriginXY.x, BubbleOriginXY.y, BubbleRadius * 0.8 , CurrentSaturateColour); //The colour preview is 80% of the bubble, the rest is outline
}



std::map<int, std::vector<int>> ColourDial::GenerateRGBTuples()
{
    //Take a base RGB tuple RED [255, 0, 0] and use it to generate all other saturated colours
    std::map<int, std::vector<int>> MapOfRGBTuples {};

    std::vector BGRTuple {0, 0, 255}; //In order to match the math with the pattern below, the RGB values are inverted to BGR
    int PatternArray[6] {0, 0, 1, 0, 0, -1}; //All channels follow this pattern: rise from 0 to 255, stay, drop from 255 to 0, stay, repeat

    for(int RGBStep {0}; RGBStep < BandsAmount; ++RGBStep) //6 zones of 255 bands each, means 1530 total bands
    {
        MapOfRGBTuples[RGBStep] = BGRTuple;
        for(int RGBValue {0}; RGBValue < 3; ++RGBValue)
        {
            BGRTuple[RGBValue] += PatternArray[(int(RGBStep / RGBValMax) + RGBValue * 2)% 6];
        }
    }
    return MapOfRGBTuples;
}


std::map<int, std::vector<float>> ColourDial::CalculateDialPositions()
{ 
    //Generate a vector of coordinates to draw lines with for the dial {XStart, XEnd, YStart, YEnd}
    //Essentially 1530 points lined up in a circle, radiating outwards

    std::map<int, std::vector<float>> DialXYPositions {};

    for(int CircleStep {0}; CircleStep < BandsAmount; ++CircleStep)
    {
        float UnitCircleSegment = float((2. * PI)/float(BandsAmount)) * float(CircleStep);
 
        /*
        Think of the dial as two unit circles, scaled up by a value, where each x coordinate is the cosine of 2 pi / total colour bands.
        Ditto for y coordinates with sine

        This means that for every band there now exists a point on the inner circle, as well as a point on the outer circle
        between which a line can be drawn to create a thick dial

        The offsets can then be used to move this dial around the screen
        */
        DialXYPositions[CircleStep] = {
                                        (std::cos(UnitCircleSegment) * DialInnerRadius) + DialOriginXY.x, //XStart
                                        (std::cos(UnitCircleSegment) * DialOuterRadius) + DialOriginXY.x, //XEnd
                                        (std::sin(UnitCircleSegment) * DialInnerRadius) + DialOriginXY.y, //YStart
                                        (std::sin(UnitCircleSegment) * DialOuterRadius) + DialOriginXY.y  //YEnd
                                      };
    }
    return DialXYPositions;
}


int ColourDial::GetRGBColour(Vector2 MouseXY, float DistanceToClick)
{
    //Get the precise RGB colour value when the colour dial is clicked

    int i_RGBSaturatesMap {0}; //MapOfRGBSaturates lookup value
    
    //Now this looks really obtuse, but all it does it convert the x-coordinate back to a point on a standard unit circle
    //and then scale it back from 0-2PI to 0-BandsAmount, e.g. 0-1530
    //This value then acts as the lookup key for the RGB colour tuples in the MapOfRGBSaturates
    //Since x is 1-dimensional, we can check for the "top" or "bottom" of the circle by seeing if the y is negative, if so we need to act as if counting pi-2pi
    if(MouseXY.y - DialOriginXY.y < 0)
    {
        i_RGBSaturatesMap = int(((acos((MouseXY.x - DialOriginXY.x) / DistanceToClick)) / (2 * PI)) * BandsAmount);
    }
    else
    {
        i_RGBSaturatesMap = int((((PI - acos((MouseXY.x - DialOriginXY.x) / DistanceToClick)) + PI) / (2 * PI)) * BandsAmount);
    }
    return i_RGBSaturatesMap;
}


void ColourDial::UpdateBubblePosition()
{
    //Move the Dial's Bubble to the clicked position to preview the selected saturate

    //At any point, the bubble's Radius is half the distance between the two bounding circles of the Dial.
    BubbleRadius = (DialOuterRadius - DialInnerRadius)/2;

    //Essentially, convert the 0-1530 Current_iRGB back into a XY point on a unit circle, scale it up by the radii and offset it with the origin
    BubbleOriginXY =    {
                            (std::cos(((2 * PI) / BandsAmount) * Current_iRGB)) * (DialInnerRadius + BubbleRadius) + DialOriginXY.x,
                            (std::sin(((2 * PI) / BandsAmount) * Current_iRGB)) * (DialInnerRadius + BubbleRadius) + DialOriginXY.y
                        };
}

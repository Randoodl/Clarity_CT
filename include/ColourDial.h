#pragma once

#include <map>

#include "./ShadeSquare.h"
#include "./Constants.h"

class ColourDial
{
    //This class is meant to hold all the information for the Hue selection dial
    
    public:
        ColourDial();

        void Update(int SetOriginX, int SetOriginY, int SetOuterRadius);
        void DrawRGBDial();

        Color GetSaturateColour(Vector2 MouseXY);
        
        Vector3 GetSquareInDialOffsets();   //This is needed to calculate the anchor point and length for the ShadeSquare
        
        Color CurrentSaturateColour;

        float DialBandThickness;
        Vector2 DialOriginXY;
        int DialInnerRadius;
        int DialOuterRadius;

    private:
        Color ElementOutLines;
        int Current_iRGB;   //The current index in the RGBSaturates map that updates when a colour is selected

        int BandsAmount; //More or less forced to 1530, 3 values in RGB, 2 states each (rising, falling), between 0-255 totals (6 *255) 1530 possible bands
        
        std::map<int, std::vector<float>> MapOFDialPositions;  //Vectors of X and Y coordinates to draw lines between, creating a dial
        std::map<int, std::vector<int>> MapOfRGBSaturates;     //All 1530 possible saturated RGB colours for each R,G or B value from 0-255

        int GetRGBColour(Vector2 MouseXY, float DistanceToClick);
        std::map<int, std::vector<int>> GenerateRGBTuples();
        std::map<int, std::vector<float>> CalculateDialPositions();

        Vector2 BubbleOriginXY;
        float BubbleRadius;  
        void UpdateBubblePosition();
};
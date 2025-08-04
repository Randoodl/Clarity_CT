#pragma once

#include <map>

#include "./ShadeSquare.h"

class ColourDial
{
    //This class is meant to hold all the information for the fully saturated colour dial
    
    public:
        ColourDial();

        void Update(int SetOriginX, int SetOriginY, int SetOuterRadius);
        void DrawRGBDial();
        void UpdateRGBSquareColour(Vector2 MouseXY);
        void UpdateBubblePosition();

        Color CurrentShadeColour;
        ShadeSquare RGBSquare;   

        Rectangle ShadeSelectSquare;

    private:
        Color ElementOutLines;
        int Current_iRGB;   //The current index in the RGBSaturates map that updates when a colour is selected

        int BandsAmount;
        float DialBandThickness;
        Vector2 DialOriginXY;
        int DialInnerRadius;
        int DialOuterRadius;
        
        std::map<int, std::vector<float>> MapOFDialPositions;  //Vectors of X and Y coordinates to draw lines between, creating a dial
        std::map<int, std::vector<int>> MapOfRGBSaturates;     //All 1530 possible saturated RGB colours for each R,G or B value from 0-255

        int GetRGBColour(Vector2 MouseXY, float DistanceToClick);
        std::map<int, std::vector<int>> GenerateRGBTuples();
        std::map<int, std::vector<float>> CalculateDialPositions();

        Vector2 BubbleOriginXY;
        float BubbleRadius;  
};
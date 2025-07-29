#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <cmath>

#include "./GradientSquare.h"

class ColourDial
{
    //This class is meant to hold all the information for the fully saturated colour dial
    
    public:
        ColourDial();

        int BandsAmount;
        int DialOriginX;
        int DialOriginY;
        int DialInnerRadius;
        int DialOuterRadius;

        std::map<int, std::vector<float>> MapOFDialPositions;  //Vectors of X and Y coordinates to draw lines between, creating a dial
        std::map<int, std::vector<int>> MapOfRGBSaturates;     //All 1530 possible saturated RGB colours for each R,G or B value from 0-255

        void Update(int SetOriginX, int SetOriginY, int SetOuterRadius);
        void PrintValues();
        void DrawRGBDial();
        void UpdateRGBSquareColour(Vector2 MouseXY);

        GradientSquare RGBSquare;
        
    private:
        float DialBandThickness;

        int GetRGBColour(Vector2 MouseXY, float DistanceToClick);

        std::map<int, std::vector<int>> GenerateRGBTuples();
        std::map<int, std::vector<float>> CalculateDialPositions();
        
};
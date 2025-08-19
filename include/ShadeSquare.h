#pragma once

#include <raylib.h>
#include <cmath>
#include <vector>

#include "./Constants.h"

class ShadeSquare
{
    //This class is meant to hold all the information for the shade square in the centre of the colour dial

    public:
        ShadeSquare();
        Rectangle ShadeSquareRectangle;

        //Everything related to generating the texture for the Shade Square
        Color SquareBaseColour;
        Color ShadedColour;
        std::vector<std::vector<Color>> GetVectorOfPixels();
        void ConvertVectorToTexture(const std::vector<std::vector<Color>>& VectorOfPixels);
        Texture2D ShadedImage;
        bool ShadedImageIsLoaded; 

        void Update(Rectangle TotalFrameArea);
        void DrawShadeSquare();
        Color GetSquareRGB(Vector2 MouseXY, Color SeedColour);

        Vector2 CurrentShadeMouseLocation;

    private:
        Vector2 ShadeViewBoxXY;
        float ShadeViewBoxDimensions;
        void UpdateShadeViewBoxPosition(Vector2 MouseXY);
        Color ShadeViewBoxOutline;
        Vector2 GetCorrectedMouseXY(Vector2 MouseXY);
};
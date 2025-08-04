#include "../include/ShadeSquare.h"


ShadeSquare::ShadeSquare()
{
    SquareBaseColour = {255, 0, 0, 255};
    ShadedImageIsLoaded = false;   
}


void ShadeSquare::Update(Rectangle TotalFrameArea)
{
    XAnchorPoint = TotalFrameArea.x;
    YAnchorPoint = TotalFrameArea.y;
    SquareEdgeLength = TotalFrameArea.height;
    CurrentShadeMouseLocation = {float(XAnchorPoint), float(YAnchorPoint)};
    ConvertVectorToTexture(GetVectorOfPixels());
}


std::vector<std::vector<Color>> ShadeSquare::GetVectorOfPixels()
{
    //Turn a ShadeSquare into a collection of pixels that can be redrawn for each frame, initialise it to 255x255
    std::vector<std::vector<Color>> VectorOfPixels(255, std::vector<Color>(255));

    //Create a vector to hold the colour as floats (to avoid narrowing fuckery)
    std::vector<float> ShadeRGBFloats = {float(SquareBaseColour.r), float(SquareBaseColour.g), float(SquareBaseColour.b)};
    
    //The proportional difference from the R, G or B value to 0, per 1 step in 255 total steps
    float ShadingFactorRed   = ShadeRGBFloats[0] / 255.;
    float ShadingFactorGreen = ShadeRGBFloats[1] / 255.;
    float ShadingFactorBlue  = ShadeRGBFloats[2] / 255.;

    for(int ShaderStep {0}; ShaderStep < 255; ++ShaderStep)
    {
        //Update the row to be slightly darker than the last one
        ShadeRGBFloats[0] -= ShadingFactorRed;  
        ShadeRGBFloats[1] -= ShadingFactorGreen;  
        ShadeRGBFloats[2] -= ShadingFactorBlue;   

        //Create an empty colour to store the per-pixel colour value
        Color ShadeColour = {0,0,0,255};

        //Create a vector to hold the colour as floats
        std::vector<float> TintRGBFloats = {float(ShadeRGBFloats[0]), float(ShadeRGBFloats[1]), float(ShadeRGBFloats[2])};

        //The proportional difference from the R, G or B value to the highest RGB (255 - Darkstep), per 1 step in 255 total steps
        float TintingFactorRed   = ((255 - ShaderStep) - TintRGBFloats[0]) / 255;
        float TintingFactorGreen = ((255 - ShaderStep) - TintRGBFloats[1]) / 255;
        float TintingFactorBlue  = ((255 - ShaderStep) - TintRGBFloats[2]) / 255;

        for(int TinterStep {0}; TinterStep < 255; ++TinterStep)
        {
            //Update the Row-Column pair (pixel) to be slightly lighter
            TintRGBFloats[0] += TintingFactorRed;
            TintRGBFloats[1] += TintingFactorGreen;
            TintRGBFloats[2] += TintingFactorBlue;

            ShadeColour.r = TintRGBFloats[0];
            ShadeColour.g = TintRGBFloats[1];
            ShadeColour.b = TintRGBFloats[2];

            VectorOfPixels[ShaderStep][TinterStep] = ShadeColour;
            //DrawRectangle(XAnchorPoint + (EdgePerBand * TinterStep), YAnchorPoint + (EdgePerBand * ShaderStep), std::ceil(BandThickness), std::ceil(BandThickness), ShadeColour);
        }               
    }
    return VectorOfPixels;
}


void ShadeSquare::ConvertVectorToTexture(const std::vector<std::vector<Color>>& VectorOfPixels)
{
    //Turn the vector of pixel colours into a texture
    //This is some voodoo that I need to delve into deeper to fully understand

    int Height = VectorOfPixels.size();
    int Width  = VectorOfPixels[0].size();

    //Allocate memory to hold all the pixel data
    //RL_MALLOC() is Raylib's malloc() and we are setting aside the whole 2D vector * 4 bytes, as each pixel is [RGBA] or 4 bytes.
    unsigned char* RawPixelData = (unsigned char*)RL_MALLOC(Width * Height * 4);

    //Raylib stores pixel data as a single line, so turning everything into a 2D vector is probably stupid, now we're just turning it back to a single line, what was the point?
    for(int Row {0}; Row < Height; ++Row)
    {
        for(int Column {0}; Column < Width; ++Column)
        {
            Color ColourOfPixel = VectorOfPixels[Row][Column];

            int IndexInSingleString = (Row * Width + Column) * 4;     //Essentially starting a new [RGBA] block in the single stream string
            RawPixelData[IndexInSingleString]     = ColourOfPixel.r;  //[R]
            RawPixelData[IndexInSingleString + 1] = ColourOfPixel.g;  //[G]
            RawPixelData[IndexInSingleString + 2] = ColourOfPixel.b;  //[B]
            RawPixelData[IndexInSingleString + 3] = ColourOfPixel.a;  //[A]
        }
    }

    //Now we have turned the 2D Vector into a single string of [RGBA] blocks, let's load it into a texture
    Image ConvertedImage = {
                            .data = RawPixelData,
                            .width = Width,
                            .height = Height,
                            .mipmaps = 1,  //mipmaps are not strictly needed, but better to set it to a safe value regardless
                            .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
                           };

    //Now size it to the actual frame size within the dial
    ImageResize(&ConvertedImage, SquareEdgeLength, SquareEdgeLength);

    //And finally, turn it into a texture, free the used memory and return the texture
    if(ShadedImageIsLoaded)
    {
        //If a texture already exists, unload it to avoid GPU leaks
        UnloadTexture(ShadedImage);
        ShadedImageIsLoaded = false;
    }

    ShadedImage = LoadTextureFromImage(ConvertedImage);   //Load Image into the texture
    ShadedImageIsLoaded = true;                           //Set memory flag
    UnloadImage(ConvertedImage);                          //Free image memory for use down the line
}


void ShadeSquare::DrawShadeSquare()
{
    DrawTexturePro(
        ShadedImage, //The generated texture
        {0, 0, float(ShadedImage.width), float(ShadedImage.height)},  //what area of the texture to use, in this case all of it
        {float(XAnchorPoint), float(YAnchorPoint), float(SquareEdgeLength), float(SquareEdgeLength)}, //from which point to project the texture and at what scale 
        {0, 0}, //Origin for rotational movement, not needed here so set to 0
        0.0f, //Rotation angle, set to zero due to no need
        WHITE //No tinting needed, set to White
    );
}


Color ShadeSquare::GetSquareRGB(Vector2 MouseXY)
{
    //Turn the mouse's XY coordinates back into a colour doing a bunch of math translations
    //Essentially the inverse of DrawShadeSquare()

    //Storing this for Dial feedback use
    CurrentShadeMouseLocation = MouseXY;

    //Color to update and return
    Color CalculatedColour = SquareBaseColour;

    //The mouse distance within the square translated to a value between 0-255, from top left to bottom right
    int RelativeX = ((MouseXY.x - XAnchorPoint) / SquareEdgeLength) * 255.;
    int RelativeY = ((MouseXY.y - YAnchorPoint) / SquareEdgeLength) * 255.;

    //Updating the colour in the Y direction, making it lighter (tint)
    float TintingFactorRed   = ((255. - CalculatedColour.r) / 255.);
    float TintingFactorGreen = ((255. - CalculatedColour.g) / 255.);
    float TintingFactorBlue  = ((255. - CalculatedColour.b) / 255.);

    CalculatedColour.r += (TintingFactorRed * RelativeX);
    CalculatedColour.g += (TintingFactorGreen * RelativeX);
    CalculatedColour.b += (TintingFactorBlue * RelativeX);

    //Updating the colour in the X direction, making it darker (shade)
    float ShadingFactorRed   = (CalculatedColour.r / 255.);
    float ShadingFactorGreen = (CalculatedColour.g / 255.);
    float ShadingFactorBlue  = (CalculatedColour.b / 255.);

    CalculatedColour.r -= (ShadingFactorRed * RelativeY);
    CalculatedColour.g -= (ShadingFactorGreen * RelativeY);
    CalculatedColour.b -= (ShadingFactorBlue * RelativeY);
    
    return CalculatedColour;
}
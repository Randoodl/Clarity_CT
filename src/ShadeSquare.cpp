#include "../include/ShadeSquare.h"


ShadeSquare::ShadeSquare()
{
    ShadeSquareRectangle = {0, 0, 0, 0};
    SquareBaseColour = {255, 0, 0, 255};
    ShadedColour = SquareBaseColour;
    ShadedImage = {};
    ShadedImageIsLoaded = false;   
    CurrentShadeMouseLocation = {0, 0};
    ShadeViewBoxXY = {0, 0};
    ShadeViewBoxDimensions = 0;
    ShadeViewBoxOutline = {0, 0, 0, 255};
}


void ShadeSquare::Update(Rectangle TotalFrameArea)
{
    //RGBSquare will always take up the whole of RGBSquareFrame
    //The frame itself is relative to RGBColourDial
    ShadeSquareRectangle.x = TotalFrameArea.x;
    ShadeSquareRectangle.y  = TotalFrameArea.y;
    ShadeSquareRectangle.width = ShadeSquareRectangle.height = TotalFrameArea.height;
    UpdateShadeViewBoxPosition(CurrentShadeMouseLocation);
    ConvertVectorToTexture(GetVectorOfPixels());
}


std::vector<std::vector<Color>> ShadeSquare::GetVectorOfPixels()
{
    //Turn a ShadeSquare into a collection of pixels that can be redrawn for each frame, initialise it to 255x255
    
    //Two dimensional Color array of size 255x255 to store all Possible colours
    std::vector<std::vector<Color>> VectorOfPixels(RGBValMax, std::vector<Color>(RGBValMax));

    //Create a vector to hold the colour as floats (to avoid narrowing fuckery)
    float ShadeRGBFloats[3] = {float(SquareBaseColour.r), float(SquareBaseColour.g), float(SquareBaseColour.b)};
    
    //The proportional difference from the R, G or B value to 0, per 1 step in 255 total steps
    float ShadingFactorRed   = ShadeRGBFloats[0] / float(RGBValMax);
    float ShadingFactorGreen = ShadeRGBFloats[1] / float(RGBValMax);
    float ShadingFactorBlue  = ShadeRGBFloats[2] / float(RGBValMax);

    for(int ShaderStep {0}; ShaderStep < RGBValMax; ++ShaderStep)
    {
        //Update the row to be slightly darker than the last one
        ShadeRGBFloats[0] -= ShadingFactorRed;  
        ShadeRGBFloats[1] -= ShadingFactorGreen;  
        ShadeRGBFloats[2] -= ShadingFactorBlue;   

        //Create an empty colour to store the per-pixel colour value
        Color ShadeColour = {0,0,0,255};

        //Create a vector to hold the colour as floats
        float TintRGBFloats[3] = {float(ShadeRGBFloats[0]), float(ShadeRGBFloats[1]), float(ShadeRGBFloats[2])};

        //The proportional difference from the R, G or B value to the highest RGB (255 - Darkstep), per 1 step in 255 total steps
        float TintingFactorRed   = ((RGBValMax - ShaderStep) - TintRGBFloats[0]) / RGBValMax;
        float TintingFactorGreen = ((RGBValMax - ShaderStep) - TintRGBFloats[1]) / RGBValMax;
        float TintingFactorBlue  = ((RGBValMax - ShaderStep) - TintRGBFloats[2]) / RGBValMax;

        for(int TinterStep {0}; TinterStep < RGBValMax; ++TinterStep)
        {
            //Update the Row-Column pair (pixel) to be slightly lighter
            TintRGBFloats[0] += TintingFactorRed;
            TintRGBFloats[1] += TintingFactorGreen;
            TintRGBFloats[2] += TintingFactorBlue;

            ShadeColour.r = TintRGBFloats[0];
            ShadeColour.g = TintRGBFloats[1];
            ShadeColour.b = TintRGBFloats[2];

            VectorOfPixels[ShaderStep][TinterStep] = ShadeColour;
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
    ImageResize(&ConvertedImage, ShadeSquareRectangle.width, ShadeSquareRectangle.height);

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
    DrawTexture(ShadedImage, ShadeSquareRectangle.x, ShadeSquareRectangle.y, WHITE);

    //Draw the ShadeBox to preview the selected shade
    int OutLineOffset = ShadeViewBoxDimensions * 0.1; 
    if(OutLineOffset < 1){OutLineOffset = 1;} //Stops it from disappearing if scaled down to miniscule size, why would anyone do this? I have no idea. But let's handle it anyway
    DrawRectangle(ShadeViewBoxXY.x, ShadeViewBoxXY.y, ShadeViewBoxDimensions, ShadeViewBoxDimensions , ShadeViewBoxOutline);
    DrawRectangle(ShadeViewBoxXY.x + OutLineOffset, ShadeViewBoxXY.y + OutLineOffset, 
                  ShadeViewBoxDimensions - OutLineOffset * 2, ShadeViewBoxDimensions - OutLineOffset * 2, GetSquareRGB(CurrentShadeMouseLocation, SquareBaseColour));
}


Vector2 ShadeSquare::GetCorrectedMouseXY(Vector2 MouseXY)
{
    //This restricts the cursor's movement to the ShadeSquare
    //MouseXY cannot be corrected independently since ToolContainer.InteractWithRGBDial() calls this as well,
    //it needs to be called with GetSquareRGB

    //Honestly, this is just me having dumb fun. Will it come back to bite me in the ass? Oh definitely, but that is for future me to deal with
    //This could all be done with a bunch of if statements, but where is the fun in that? It is essentially just one sum per coordinate where:
    //
    //  "Is too low"     *   minimum value     
    //  "Is too high"    *   maximum value
    //  "Is just right"  *   current value
    //
    //Since only one of these conditionials can be right at one time, only the minimum, maximum or current value is set
    //Yes, it is needless and dumb, but it was fun rigging it up like this

    MouseXY.x = (
        ((MouseXY.x < ShadeSquareRectangle.x) * ShadeSquareRectangle.x) +                                                               
        ((MouseXY.x > (ShadeSquareRectangle.x + ShadeSquareRectangle.width)) * (ShadeSquareRectangle.x + ShadeSquareRectangle.width)) +
        (((MouseXY.x >= ShadeSquareRectangle.x) && (MouseXY.x <= (ShadeSquareRectangle.x + ShadeSquareRectangle.width))) * MouseXY.x));

    MouseXY.y = (
        ((MouseXY.y < ShadeSquareRectangle.y) * ShadeSquareRectangle.y) +                                                               
        ((MouseXY.y > (ShadeSquareRectangle.y + ShadeSquareRectangle.height)) * (ShadeSquareRectangle.y + ShadeSquareRectangle.height)) +
        (((MouseXY.y >= ShadeSquareRectangle.y) && (MouseXY.y <= (ShadeSquareRectangle.y + ShadeSquareRectangle.height))) * MouseXY.y));
    
    return MouseXY;
}


Color ShadeSquare::GetSquareRGB(Vector2 MouseXY, Color SeedColour)
{
    //Turn the mouse's XY coordinates back into a colour doing a bunch of math translations
    //Essentially the inverse of DrawShadeSquare()

    if(!CheckCollisionPointRec(MouseXY, ShadeSquareRectangle))
    {
        MouseXY = GetCorrectedMouseXY(MouseXY);
    }

    //Storing this for Dial feedback use
    CurrentShadeMouseLocation = MouseXY;

    //Color to update and return
    Color CalculatedColour = SeedColour;

    //The mouse distance within the square translated to a value between 0-255, from top left to bottom right
    int RelativeX = ((MouseXY.x - ShadeSquareRectangle.x) / ShadeSquareRectangle.width) * RGBValMax;
    int RelativeY = ((MouseXY.y - ShadeSquareRectangle.y) / ShadeSquareRectangle.height) * RGBValMax;

    //Updating the colour in the Y direction, making it lighter (tint)
    float TintingFactorRed   = ((float(RGBValMax) - CalculatedColour.r) / float(RGBValMax));
    float TintingFactorGreen = ((float(RGBValMax) - CalculatedColour.g) / float(RGBValMax));
    float TintingFactorBlue  = ((float(RGBValMax) - CalculatedColour.b) / float(RGBValMax));

    CalculatedColour.r += (TintingFactorRed * float(RelativeX));
    CalculatedColour.g += (TintingFactorGreen * float(RelativeX));
    CalculatedColour.b += (TintingFactorBlue * float(RelativeX));

    //Updating the colour in the X direction, making it darker (shade)
    float ShadingFactorRed   = (CalculatedColour.r / float(RGBValMax));
    float ShadingFactorGreen = (CalculatedColour.g / float(RGBValMax));
    float ShadingFactorBlue  = (CalculatedColour.b / float(RGBValMax));

    CalculatedColour.r -= (ShadingFactorRed * float(RelativeY));
    CalculatedColour.g -= (ShadingFactorGreen * float(RelativeY));
    CalculatedColour.b -= (ShadingFactorBlue * float(RelativeY));
    
    UpdateShadeViewBoxPosition(MouseXY);

    return CalculatedColour;
}


void ShadeSquare::UpdateShadeViewBoxPosition(Vector2 MouseXY)
{
    //Moves the Shade Preview Box around the Shade Square when clicked and dragged
    ShadeViewBoxDimensions = ShadeSquareRectangle.height / 10;
    ShadeViewBoxXY.x = MouseXY.x - (ShadeViewBoxDimensions/2);
    ShadeViewBoxXY.y = MouseXY.y - (ShadeViewBoxDimensions/2);

    //This makes the outline lighter/darker based on the Y postion in the square, keeping it visible
    int ShadeFactor = (RGBValMax/(ShadeSquareRectangle.height + ShadeViewBoxDimensions)) * (ShadeViewBoxXY.y - ShadeSquareRectangle.y + ShadeViewBoxDimensions);
    ShadeViewBoxOutline = {0, 0, 0, 255}; //Stops outline from just continuously looping through all grey tones
    ShadeViewBoxOutline.r += ShadeFactor;
    ShadeViewBoxOutline.g += ShadeFactor;
    ShadeViewBoxOutline.b += ShadeFactor;
}
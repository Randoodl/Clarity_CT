#include "../include/ShadeSquare.h"
#include <iostream>

ShadeSquare::ShadeSquare()
{
    SquareBaseColour = {255, 0, 0, 255};
    ShadedImageIsLoaded = false;   
}


void ShadeSquare::Update(Rectangle TotalFrameArea)
{
    ShadeSquareRectangle.x = TotalFrameArea.x;
    ShadeSquareRectangle.y  = TotalFrameArea.y;
    ShadeSquareRectangle.width = ShadeSquareRectangle.height = TotalFrameArea.height;
    UpdateShadeViewBoxPosition(CurrentShadeMouseLocation);
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
    DrawRectangle(ShadeViewBoxXY.x, ShadeViewBoxXY.y, ShadeViewBoxDimensions, ShadeViewBoxDimensions , ShadeViewBoxOutline);
    DrawRectangle(ShadeViewBoxXY.x + OutLineOffset, ShadeViewBoxXY.y + OutLineOffset, 
                  ShadeViewBoxDimensions - OutLineOffset * 2, ShadeViewBoxDimensions - OutLineOffset * 2, GetSquareRGB(CurrentShadeMouseLocation));
}


Vector2 ShadeSquare::GetCorrectedMouseXY(Vector2 MouseXY)
{
    //This restricts the cursor's movement to the ShadeSquare
    //MouseXY cannot be corrected independently since ToolContainer.InteractWithRGBDial()
    //calls this as well, it needs to be called with GetSquareRGB

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


Color ShadeSquare::GetSquareRGB(Vector2 MouseXY)
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
    Color CalculatedColour = SquareBaseColour;

    //The mouse distance within the square translated to a value between 0-255, from top left to bottom right
    int RelativeX = ((MouseXY.x - ShadeSquareRectangle.x) / ShadeSquareRectangle.width) * 255;
    int RelativeY = ((MouseXY.y - ShadeSquareRectangle.y) / ShadeSquareRectangle.height) * 255;

    //Updating the colour in the Y direction, making it lighter (tint)
    float TintingFactorRed   = ((255. - CalculatedColour.r) / 255.);
    float TintingFactorGreen = ((255. - CalculatedColour.g) / 255.);
    float TintingFactorBlue  = ((255. - CalculatedColour.b) / 255.);

    CalculatedColour.r += (TintingFactorRed * float(RelativeX));
    CalculatedColour.g += (TintingFactorGreen * float(RelativeX));
    CalculatedColour.b += (TintingFactorBlue * float(RelativeX));

    //Updating the colour in the X direction, making it darker (shade)
    float ShadingFactorRed   = (CalculatedColour.r / 255.);
    float ShadingFactorGreen = (CalculatedColour.g / 255.);
    float ShadingFactorBlue  = (CalculatedColour.b / 255.);

    CalculatedColour.r -= (ShadingFactorRed * float(RelativeY));
    CalculatedColour.g -= (ShadingFactorGreen * float(RelativeY));
    CalculatedColour.b -= (ShadingFactorBlue * float(RelativeY));
    
    UpdateShadeViewBoxPosition(MouseXY);

    return CalculatedColour;
}


void ShadeSquare::UpdateShadeViewBoxPosition(Vector2 MouseXY)
{
    //TODO: Currently snaps box to top left during ColourDial scaling
    //      because the scaling calls ShadeSquare.Update, which calls UpDateViewBoxLocation with CurrentShadeMouseLocation
    //      which, in this update, is set to to X and Y anchor points of the Shade Square, hence it snaps to the top left

    //Moves the Shade Preview Box around the Shade Square when clicked and dragged
    ShadeViewBoxDimensions = ShadeSquareRectangle.height / 10;
    ShadeViewBoxXY.x = MouseXY.x - (ShadeViewBoxDimensions/2);
    ShadeViewBoxXY.y = MouseXY.y - (ShadeViewBoxDimensions/2);

    //This makes the outline lighter/darker based on the Y postion in the square, keeping it visible
    ShadeViewBoxOutline = {45, 45, 45, 255};
    int ShadeFactor = (255./ShadeSquareRectangle.height) * (ShadeViewBoxXY.y - ShadeSquareRectangle.y) * 0.8;
    ShadeViewBoxOutline.r += (ShadeFactor);
    ShadeViewBoxOutline.g += (ShadeFactor);
    ShadeViewBoxOutline.b += (ShadeFactor);
}
#include "../include/ColourDial.h"

ColourDial::ColourDial() 
{
    BandsAmount = 1530;  //More or less forced, 3 values in RGB, 2 states each (rising, falling), between 0-255 totals (6 *255) 1530 possible bands
    MapOfRGBSaturates = GenerateRGBTuples();  
}


void ColourDial::Initialise(int SetOriginX, int SetOriginY, int SetOuterRadius)
{
    //When the program is started, load initial conditions and apply them
    //Perhaps UI save function in the future?

    DialOriginX = SetOriginX;
    DialOriginY = SetOriginY;
    DialInnerRadius = SetOuterRadius - (SetOuterRadius/5); //Approach the Dial's max size from the Frame holding it
    DialOuterRadius = SetOuterRadius;
    DialBandThickness = (DialOuterRadius - DialInnerRadius) * 0.1; //10% of the Dial's thickness is a wild guess, might update later

    RGBSquare.Initialise(DialOriginX, DialOriginY, DialInnerRadius);

    MapOFDialPositions = CalculateDialPositions();  
}


std::map<int, std::vector<int>> ColourDial::GenerateRGBTuples()
{
    //Take a base RGB tuple RED [255, 0, 0] and use it to generate all other saturated colours
    std::map<int, std::vector<int>> MapOfRGBTuples {};

    std::vector BGRTuple {0, 0, 255}; //In order to match the math with the pattern below, the RGB values are inverted to BGR
    int PatternArray[6] {0, 0, 1, 0, 0, -1};

    for(int RGBStep {0}; RGBStep < BandsAmount; ++RGBStep) //6 zones of 255 bands each, means 1530 total bands
    {
        MapOfRGBTuples[RGBStep] = BGRTuple;
        for(int RGBValue {0}; RGBValue < 3; ++RGBValue)
        {
            BGRTuple[RGBValue] += PatternArray[(int(RGBStep / 255) + RGBValue * 2)% 6];
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
                                        (std::cos(UnitCircleSegment) * DialInnerRadius) + DialOriginX, //XStart
                                        (std::cos(UnitCircleSegment) * DialOuterRadius) + DialOriginX, //XEnd
                                        (std::sin(UnitCircleSegment) * DialInnerRadius) + DialOriginY, //YStart
                                        (std::sin(UnitCircleSegment) * DialOuterRadius) + DialOriginY  //YEnd
                                      };
    }
    return DialXYPositions;
}


void ColourDial::DrawRGBDial()
{
    //Draw a colour dial based on the data calculated in the MapOfDialPositions

    Color BaseColour {0 ,0 ,0 ,255}; //Placeholder colour to update and draw to screen

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

    DrawRectangle(RGBSquare.XAnchorPoint, RGBSquare.YAnchorPoint, RGBSquare.SquareEdgeLength, RGBSquare.SquareEdgeLength, RGBSquare.SquareBaseColour);
}


void ColourDial::PrintValues()
{
    //For debugging purposes only
    for(int i {0}; i < BandsAmount; ++i)
    {
        std::cout << i << ": ";
        for(int iRGB {0}; iRGB < 3; ++ iRGB)
        {
            std::cout << MapOfRGBSaturates[i][iRGB] << " ";
        }
        std::cout << "\n";
    }
}


#include "../include/ColourDial.h"

ColourDial::ColourDial() 
{
    BandsAmount = 1530;  //More or less forced, 3 values in RGB, 2 states each (rising, falling), between 0-255 totals (6 *255) 1530 possible bands
    MapOfRGBSaturates = GenerateRGBTuples(); 
    Current_iRGB = 0;
    ElementOutLines =  {80, 80, 80, 255};
}


void ColourDial::Update(int SetOriginX, int SetOriginY, int SetOuterRadius)
{
    //When the program is started, load initial conditions and apply them
    //Perhaps UI save function in the future?

    DialOriginXY.x = SetOriginX;
    DialOriginXY.y = SetOriginY;
    DialInnerRadius = SetOuterRadius - (SetOuterRadius/3); //Approach the Dial's max size from the Frame holding it
    DialOuterRadius = SetOuterRadius;
    DialBandThickness = (DialOuterRadius - DialInnerRadius) * 0.1; //10% of the Dial's thickness is a wild guess, might update later

    RGBSquare.Update(DialOriginXY.x, DialOriginXY.y, DialInnerRadius);
    UpdateBubblePosition();

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
                                        (std::cos(UnitCircleSegment) * DialInnerRadius) + DialOriginXY.x, //XStart
                                        (std::cos(UnitCircleSegment) * DialOuterRadius) + DialOriginXY.x, //XEnd
                                        (std::sin(UnitCircleSegment) * DialInnerRadius) + DialOriginXY.y, //YStart
                                        (std::sin(UnitCircleSegment) * DialOuterRadius) + DialOriginXY.y  //YEnd
                                      };
    }
    return DialXYPositions;
}


void ColourDial::DrawRGBDial()
{
    //Draw a colour dial based on the data calculated in the MapOfDialPositions

    Color BaseColour {0, 0, 0, 255}; //Placeholder colour to update and draw to screen

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

    //Draw the Gradient Square
    DrawRectangle(RGBSquare.XAnchorPoint, RGBSquare.YAnchorPoint, RGBSquare.SquareEdgeLength, RGBSquare.SquareEdgeLength, RGBSquare.SquareBaseColour);

    //Draw the colour preview bubble
    DrawCircle(BubbleOriginXY.x, BubbleOriginXY.y, (DialOuterRadius - DialInnerRadius)/2, ElementOutLines);
    DrawCircle(BubbleOriginXY.x, BubbleOriginXY.y, BubbleRadius * 0.8 , RGBSquare.SquareBaseColour);
}


void ColourDial::UpdateRGBSquareColour(Vector2 MouseXY)
{
    //Using good ol' pythagoras we can calculate the distance from the centre of the dial to the mouseclick
    float DistanceToClick = sqrt(pow(MouseXY.x - DialOriginXY.x, 2) + pow(MouseXY.y - DialOriginXY.y, 2));

    if(DistanceToClick > DialInnerRadius && DistanceToClick < DialOuterRadius)
        {
            //The click occurs within the bounds of the dial

            Current_iRGB = (1530 - GetRGBColour(MouseXY, DistanceToClick)) % 1530;
            /* Using the remainder of GetRGBColour because the Map is created left-to-right, but the dial is created as a unit circle
               which increments to 2 PI in a counter-clockwise (right-to-left) fashion
               Using GetRGBColour directly would have colours and the bubble flipped along the Y-axis */
    
            RGBSquare.SquareBaseColour.r = MapOfRGBSaturates[Current_iRGB][2]; 
            RGBSquare.SquareBaseColour.g = MapOfRGBSaturates[Current_iRGB][1]; 
            RGBSquare.SquareBaseColour.b = MapOfRGBSaturates[Current_iRGB][0]; 
        
            UpdateBubblePosition();
        
        }
        else if (MouseXY.x > RGBSquare.XAnchorPoint && MouseXY.x < (RGBSquare.XAnchorPoint + RGBSquare.SquareEdgeLength) &&
                 MouseXY.y > RGBSquare.YAnchorPoint && MouseXY.y < (RGBSquare.YAnchorPoint + RGBSquare.SquareEdgeLength))
        {
            //The click occurs within the bounds of the square
            std::cout << "Square time!" << std::endl;
        }
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
    BubbleRadius = (DialOuterRadius - DialInnerRadius)/2;
    BubbleOriginXY =    {
                            (std::cos(((2 * PI) / BandsAmount) * Current_iRGB)) * (DialInnerRadius + BubbleRadius) + DialOriginXY.x,
                            (std::sin(((2 * PI) / BandsAmount) * Current_iRGB)) * (DialInnerRadius + BubbleRadius) + DialOriginXY.y
                        };
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


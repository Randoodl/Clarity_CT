#include "../include/ToolContainer.h"


ToolContainer::ToolContainer()
{
    RGBDial = ColourDial(); 
    RGBSquare.Initialise(RGBDial.DialOriginX, RGBDial.DialOriginY, RGBDial.DialInnerRadius);
}


void ToolContainer::LeftMouseClickHandler()
{
    Vector2 MousePositionXY = GetMousePosition();

    if((MousePositionXY.x >= RGBDial.DialBoundingBox[0] && MousePositionXY.x <= RGBDial.DialBoundingBox[1]) && 
       (MousePositionXY.y >= RGBDial.DialBoundingBox[2] && MousePositionXY.y <= RGBDial.DialBoundingBox[3]))
    {
        //The left mouse button is currently being pressed somewhere in the bounding box around the RGBdial
        UpdateGradientSquare(MousePositionXY);
    }
}


void ToolContainer::UpdateGradientSquare(Vector2 MouseXY)
{
    //Using good ol' pythagoras we can calculate the distance from the centre of the dial to the mouseclick
    double DistanceToClick = std::sqrt(std::pow(MouseXY.x - RGBDial.DialOriginX, 2) + std::pow(MouseXY.y - RGBDial.DialOriginY, 2));
        
    if(DistanceToClick > RGBDial.DialInnerRadius && DistanceToClick < RGBDial.DialOuterRadius)
    {
        //The click occurs within the bounds of the dial
        int i_RGBSatures = GetRGBColour(MouseXY, DistanceToClick);
        RGBSquare.SquareBaseColour.r = RGBDial.MapOfRGBSaturates[(1530 - i_RGBSatures) % 1530][2];
        RGBSquare.SquareBaseColour.g = RGBDial.MapOfRGBSaturates[(1530 - i_RGBSatures) % 1530][1];
        RGBSquare.SquareBaseColour.b = RGBDial.MapOfRGBSaturates[(1530 - i_RGBSatures) % 1530][0];    
    }
}


int ToolContainer::GetRGBColour(Vector2 MousePositionXY, double DistanceToClick)
{
    //Get the precise RGB colour value when the colour dial is clicked

    int i_RGBSaturatesMap {0}; //MapOfRGBSaturates lookup value
    
    //Now this looks really obtuse, but all it does it convert the x-coordinate back to a point on a standard unit circle
    //and then scale it back from 0-2PI to 0-BandsAmount, e.g. 0-1530
    //This value then acts as the lookup key for the RGB colour tuples in the MapOfRGBSaturates
    //Since x is 1-dimensional, we can check for the "top" or "bottom" of the circle by seeing if the y is negative, if so we need to act as if counting pi-2pi
    if(MousePositionXY.y - RGBDial.DialOriginY < 0)
    {
        i_RGBSaturatesMap = int(((acos((MousePositionXY.x - RGBDial.DialOriginX) / DistanceToClick)) / (2 * PI)) * RGBDial.BandsAmount);
    }
    else
    {
        i_RGBSaturatesMap = int((((PI - acos((MousePositionXY.x - RGBDial.DialOriginX) / DistanceToClick)) + PI) / (2 * PI)) * RGBDial.BandsAmount);
    }

    return i_RGBSaturatesMap;
}
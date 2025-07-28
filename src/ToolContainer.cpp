#include "../include/ToolContainer.h"


ToolContainer::ToolContainer()
{
    //Initialise Frame, Dial and Square for the Colour picker
    ColourPicker.Initialise(20, 20, 250, 250);
    RGBDial.Initialise(ColourPicker.AnchorX + ColourPicker.LenX/2, ColourPicker.AnchorY + ColourPicker.LenY/2, ColourPicker.LenX/2);
    RGBSquare.Initialise(RGBDial.DialOriginX, RGBDial.DialOriginY, RGBDial.DialInnerRadius);
}


void ToolContainer::DrawElements()
{
    //Simply combining all drawing calls
    ColourPicker.DrawFrameBox();
    RGBDial.DrawRGBDial();
    RGBSquare.DrawGradientSquare();
}


void ToolContainer::LeftMouseClickHandler()
{
    //Left Mouse Button
    Vector2 MouseXY = GetMousePosition();

    if((MouseXY.x > ColourPicker.AnchorX && MouseXY.x < (ColourPicker.LenX + ColourPicker.AnchorX)) &&
        (MouseXY.y > ColourPicker.AnchorY && MouseXY.y < (ColourPicker.LenY + ColourPicker.AnchorY)))
        {   
            //Currently clicking within the RGBDial Frame
            InteractWithRGBDial(MouseXY);
        }
}


void ToolContainer::InteractWithRGBDial(Vector2 MouseXY)
{
    //Using good ol' pythagoras we can calculate the distance from the centre of the dial to the mouseclick
    double DistanceToClick = std::sqrt(std::pow(MouseXY.x - RGBDial.DialOriginX, 2) + std::pow(MouseXY.y - RGBDial.DialOriginY, 2));
        
    if(DistanceToClick > RGBDial.DialInnerRadius && DistanceToClick < RGBDial.DialOuterRadius)
    {
        //The click occurs within the bounds of the dial
        int i_RGBSatures = GetRGBColour(MouseXY, DistanceToClick);
        RGBSquare.SquareBaseColour.r = RGBDial.MapOfRGBSaturates[(1530 - i_RGBSatures) % 1530][2]; //Using remainder of i_RGBSatures because the Map is created left-to-right
        RGBSquare.SquareBaseColour.g = RGBDial.MapOfRGBSaturates[(1530 - i_RGBSatures) % 1530][1]; //but the dial is created as a unit circle (counter clockwise or right-to left)
        RGBSquare.SquareBaseColour.b = RGBDial.MapOfRGBSaturates[(1530 - i_RGBSatures) % 1530][0]; //If we use i_Saturates directly the colours are flipped along the Y axis
    }
    else if (MouseXY.x > RGBSquare.XAnchorPoint && MouseXY.x < (RGBSquare.XAnchorPoint + RGBSquare.SquareEdgeLength) &&
             MouseXY.y > RGBSquare.YAnchorPoint && MouseXY.y < (RGBSquare.YAnchorPoint + RGBSquare.SquareEdgeLength))
    {
        //The click occurs within the bounds of the square
        std::cout << "Square time!" << std::endl;
    }
    else if ((MouseXY.x > ColourPicker.MoveButtonRoot[0] && MouseXY.x < (ColourPicker.MoveButtonRoot[0] + ColourPicker.EdgeButtonSize)) &&
             (MouseXY.y > ColourPicker.MoveButtonRoot[1] && MouseXY.y < (ColourPicker.MoveButtonRoot[1] + ColourPicker.EdgeButtonSize)))
    {   
        //The click occurs on the move button
        std::cout << "Move" << std::endl;

    }
    else if ((MouseXY.x > ColourPicker.ScaleButtonRoot[0] && MouseXY.x < ColourPicker.ScaleButtonRoot[0] + ColourPicker.EdgeButtonSize) &&
             (MouseXY.y > ColourPicker.ScaleButtonRoot[1] && MouseXY.y < ColourPicker.ScaleButtonRoot[1] + ColourPicker.EdgeButtonSize))
    {
        //The click occurs on the scale button
        std::cout << "Scale" << std::endl;
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
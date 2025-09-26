#pragma once

#include "./ElementPosition.h"


struct Defaults
{
    //A struct of all default positions to be loaded if no other position data is found
    
    ElementPosition TOOL   = ElementPosition(920, 0, 78, 390);
    ElementPosition SQUARE = ElementPosition(0, 0, 0, 0);      //This is a dummy value in order to make the .conf file reading work
    ElementPosition DIAL   = ElementPosition(0, 0, 300, 300);
    ElementPosition HUE    = ElementPosition(0, 310, 70, 80);
    ElementPosition COMP   = ElementPosition(80, 310, 70, 80);
    ElementPosition LTRIAD = ElementPosition(160, 310, 70, 80);
    ElementPosition UTRIAD = ElementPosition(230, 310, 70, 80);
    ElementPosition MAINST = ElementPosition(310, 0, 600, 70);
    ElementPosition COMPST = ElementPosition(310, 80, 600, 70);
    ElementPosition LTRIST = ElementPosition(310, 160, 600, 70);
    ElementPosition UTRIST = ElementPosition(310, 230, 600, 70);
    ElementPosition SELECT = ElementPosition(310, 310, 600, 80);
};
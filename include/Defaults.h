/* 
 * Copyright (c) 2025, Dylan Ooijevaar
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
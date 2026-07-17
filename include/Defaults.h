/* 
 * Copyright (c) 2026, Dylan Ooijevaar
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

#include "./ElementState.h"


struct Defaults
{
    //A struct of all default positions to be loaded if no other position data is found
    
    ElementState TOOL   = ElementState("TOOL", 920, 0, 65, 390);
    ElementState SQUARE = ElementState("SQUARE", 0, 0, 0, 0);      //This is a dummy value in order to make the .conf file reading work
    ElementState DIAL   = ElementState("DIAL", 0, 0, 300, 300);
    ElementState HUE    = ElementState("HUE", 0, 310, 70, 80);
    ElementState COMP   = ElementState("COMP", 80, 310, 70, 80);
    ElementState LTRIAD = ElementState("LTRIAD", 160, 310, 70, 80);
    ElementState UTRIAD = ElementState("UTRIAD", 230, 310, 70, 80);
    ElementState MAINST = ElementState("MAINST", 310, 0, 600, 70);
    ElementState COMPST = ElementState("COMPST", 310, 80, 600, 70);
    ElementState LTRIST = ElementState("LTRIST", 310, 160, 600, 70);
    ElementState UTRIST = ElementState("UTRIST", 310, 230, 600, 70);
    ElementState SELECT = ElementState("SELECT", 310, 310, 600, 80);
};
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
    
    ElementState TOOL   = ElementState(" Toolbar", 920, 0, 65, 390, true);
    ElementState SQUARE = ElementState(" NoShowSquare", 0, 0, 0, 0, true);      //This is a dummy value in order to make the .conf file reading work
    ElementState DIAL   = ElementState(" Colour Picker", 0, 0, 300, 300, true);
    ElementState HUE    = ElementState(" Base Hue", 0, 310, 70, 80, true);
    ElementState COMP   = ElementState(" Complement", 80, 310, 70, 80, true);
    ElementState LTRIAD = ElementState(" Triad 1", 160, 310, 70, 80, true);
    ElementState UTRIAD = ElementState(" Triad 2", 230, 310, 70, 80, true);
    ElementState MAINST = ElementState(" Hue Shades", 310, 0, 600, 70, true);
    ElementState COMPST = ElementState(" Complement Shades", 310, 80, 600, 70, true);
    ElementState LTRIST = ElementState(" Triadic Shades 1", 310, 160, 600, 70, true);
    ElementState UTRIST = ElementState(" Triadic Shades 2", 310, 230, 600, 70, true);
    ElementState SELECT = ElementState(" Current Colour", 310, 310, 600, 80, true);
};
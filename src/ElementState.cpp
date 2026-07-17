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

#include "../include/ElementState.h"


ElementState::ElementState(std::string SetElementTag, int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY, bool SetVisibility)
{
    ElementTag = SetElementTag;
    AnchorX = SetAnchorX;
    AnchorY = SetAnchorY;
    LenX = SetLenX;
    LenY = SetLenY;
    Visibility = SetVisibility;
}
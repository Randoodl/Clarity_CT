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

#include "./IconString.h"
#include "./Constants.h"

#include <raylib.h>
#include <vector>
#include <cmath>


class ToolBar
{
    //This class is meant to hold all the information related to the utility/tools to adjust the UI
    public:
        ToolBar();

        //The area that contains all the individual buttons
        //All buttons are placed relative to this area
        Rectangle ButtonContainer;

        //Everything needed to calculate and keep track of buttons
        Rectangle LockButton;
        Rectangle SaveButton;
        Rectangle ResetButton;
        Rectangle CodeModeButton;
        Rectangle DarkModeButton; 
       
        void Update(Rectangle& TotalFrameArea);
        void DrawToolBar(Color& SetBackGroundColour, Color& SetButtonColour, Color& SetIconColour);
        
    private:
        
        //Everything needed to calculate and display the buttons
        int ButtonMargin;
        int IconPixelAmount;
        int IconPixelDimension;

        //All Icon textures
        Texture2D LockIcon;
        Texture2D SaveIcon;
        Texture2D ResetIcon;
        Texture2D CodeModeIcon;
        Texture2D DarkModeIcon;

        //Everything needed to turn the binary icon strings into an icon texture
        IconString AllIconStrings;
        void GenerateIconTextures(std::vector<std::string*>& IconStrings, std::vector<Texture2D*>& IconTextures);
        void DrawIcon(Rectangle& ButtonArea, Texture2D* IconImage, Color& IconColour);

        //Collections of objects for for loop functionality
        std::vector<Rectangle*> Buttons;
        int ButtonAmount;
        
        std::vector<std::string*> RawIconStrings;
        std::vector<Texture2D*> Icons;      
};
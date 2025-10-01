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
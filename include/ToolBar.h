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
        Rectangle ButtonContainer;
        Rectangle LockButton;
        Texture2D LockIcon;
        Rectangle ResetButton;
        Texture2D ResetIcon;
        Rectangle SaveButton;
        Texture2D SaveIcon;
        Rectangle CodeModeButton;
        Texture2D CodeModeIcon;
        Rectangle DarkModeButton; 
        Texture2D DarkModeIcon;

        void Update(Rectangle& TotalFrameArea);
        void DrawToolBar(Color& SetBackGroundColour, Color& SetButtonColour, Color& SetIconColour);
        
    private:
        IconString AllIconStrings;

        int ButtonMargin;
        int ButtonAmount;
        int IconPixelAmount;
        int IconPixelDimension;

        std::vector<Rectangle*> Buttons;
        std::vector<Texture2D*> Icons;
        std::vector<std::string*> RawIconStrings;

        void GenerateIconTextures(std::vector<std::string*>& IconStrings, std::vector<Texture2D*>& IconTextures);
        void DrawIcon(Rectangle& ButtonArea, Texture2D* IconImage, Color& IconColour);

};
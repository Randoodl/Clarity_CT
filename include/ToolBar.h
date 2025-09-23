#pragma once

#include <raylib.h>
#include <vector>

class ToolBar
{
    public:
        ToolBar();
        Rectangle ButtonContainer;
        Rectangle LockButton;
        Rectangle ResetButton;
        Rectangle SaveButton;
        Rectangle CodeModeButton;
        Rectangle DarkModeButton; 

        void Update(Rectangle TotalFrameArea);
        void DrawToolBar(Color& SetBackGroundColour, Color& SetButtonColour);

    private:
        std::vector<Rectangle*> Buttons;
        int ButtonMargin;
        int ButtonAmount;
};
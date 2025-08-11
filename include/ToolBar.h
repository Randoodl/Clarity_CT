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

        void Update(Rectangle TotalFrameArea);
        void DrawToolBar();

    private:
        std::vector<Rectangle*> Buttons;
        Color BackgroundColour;
        Color ButtonColour;
        int ButtonMargin;
};
#include "../include/ToolBar.h"

#include <iostream>

ToolBar::ToolBar()
{
    BackgroundColour = {113, 102, 128, 255};
    ButtonColour = {133, 122, 148, 255};
    ButtonContainer = {0, 0, 0, 0};
    LockButton = {0, 0, 0, 0};
    ResetButton = {0, 0, 0, 0};
    SaveButton = {0, 0, 0, 0};
    OptionsButton = {0, 0, 0, 0};
    ButtonMargin = 5;
    Buttons = {&LockButton, &ResetButton, &SaveButton, &OptionsButton};
    ButtonAmount = Buttons.size();
}


void ToolBar::Update(Rectangle TotalFrameArea)
{
    //Setup the button container
    ButtonContainer.x = TotalFrameArea.x;
    ButtonContainer.y  = TotalFrameArea.y;

    if(TotalFrameArea.width >= TotalFrameArea.height) //Sort the bar horizontally
    {
        ButtonContainer.width = TotalFrameArea.width;
        ButtonContainer.height = ButtonContainer.width/ButtonAmount;
        if(ButtonContainer.height >= TotalFrameArea.height){ButtonContainer.height = TotalFrameArea.height;}

        //Relative button variables
        int ButtonWidth = ((ButtonContainer.width - ButtonMargin)/ButtonAmount) - ButtonMargin;
        int ButtonHeight = (ButtonContainer.height) - ButtonMargin * 2;
    
        //Cycle through all Button references and update their members respectively
        for (int IndexOfButton {0}; IndexOfButton < ButtonAmount; ++IndexOfButton)
        {
            Buttons[IndexOfButton]->x = ButtonContainer.x + ButtonMargin + (IndexOfButton * (ButtonWidth + ButtonMargin));
            Buttons[IndexOfButton]->y = ButtonContainer.y + ButtonMargin;
            Buttons[IndexOfButton]->width = ButtonWidth;
            Buttons[IndexOfButton]->height = ButtonHeight;
        }
        
    }
    else //sort the bar vertically
    {
        ButtonContainer.height = TotalFrameArea.height;
        ButtonContainer.width = ButtonContainer.height/ButtonAmount;
        if(ButtonContainer.width >= TotalFrameArea.width){ButtonContainer.width = TotalFrameArea.width;}

        //Relative button variables
        int ButtonWidth = (ButtonContainer.width) - ButtonMargin * 2;
        int ButtonHeight = ((ButtonContainer.height - ButtonMargin)/ButtonAmount) - ButtonMargin;
        

        //Cycle through all Button references and update their members respectively
        for (int IndexOfButton {0}; IndexOfButton < ButtonAmount; ++IndexOfButton)
        {
            Buttons[IndexOfButton]->x = ButtonContainer.x + ButtonMargin;
            Buttons[IndexOfButton]->y = ButtonContainer.y + ButtonMargin + (IndexOfButton * (ButtonHeight + ButtonMargin));
            Buttons[IndexOfButton]->width = ButtonWidth;
            Buttons[IndexOfButton]->height = ButtonHeight;
        }
    }    
}


void ToolBar::DrawToolBar()
{
    //Draw container background
    DrawRectangle(ButtonContainer.x, ButtonContainer.y, ButtonContainer.width, ButtonContainer.height, BackgroundColour);
    for(Rectangle* Button : Buttons)
    {
        DrawRectangle(Button->x, Button->y, Button->width, Button->height, ButtonColour);
    }
}
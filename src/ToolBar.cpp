#include "../include/ToolBar.h"


ToolBar::ToolBar()
{
    //public
    ButtonContainer = {0, 0, 0, 0};
    LockButton = {0, 0, 0, 0};
    SaveButton = {0, 0, 0, 0};
    ResetButton = {0, 0, 0, 0};
    CodeModeButton = {0, 0, 0, 0};
    DarkModeButton = {0, 0, 0, 0};

    //private
    ButtonMargin = 3;
    IconPixelAmount = 256;
    IconPixelDimension = sqrt(IconPixelAmount);
    Buttons = {&LockButton, &SaveButton, &ResetButton, &CodeModeButton, &DarkModeButton};
    ButtonAmount = Buttons.size();
    RawIconStrings = {&AllIconStrings.LockString, &AllIconStrings.SaveString, &AllIconStrings.ResetString, &AllIconStrings.CodeModeString, &AllIconStrings.DarkModeString};
    Icons = {&LockIcon, &SaveIcon, &ResetIcon, &CodeModeIcon, &DarkModeIcon};

    //This sets all Texture2Ds
    GenerateIconTextures(RawIconStrings, Icons);
}


void ToolBar::Update(Rectangle& TotalFrameArea)
{
    //Set up the button container
    ButtonContainer.x = TotalFrameArea.x;
    ButtonContainer.y  = TotalFrameArea.y;

    //Sort the bar horizontally
    if(TotalFrameArea.width >= TotalFrameArea.height) 
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


void ToolBar::DrawToolBar(Color& SetBackGroundColour, Color& SetButtonColour, Color& SetIconColour)
{
    //Draw container background
    DrawRectangle(ButtonContainer.x, ButtonContainer.y, ButtonContainer.width, ButtonContainer.height, SetBackGroundColour);
    
    //Draw the background of the button
    for(int i_Button {0}; i_Button < int(Buttons.size()); ++i_Button)
    {
        DrawRectangle(Buttons[i_Button]->x, Buttons[i_Button]->y, Buttons[i_Button]->width, Buttons[i_Button]->height, SetButtonColour);
    }

    //Draw Button Icons
    for(int i_Icon {0}; i_Icon < ButtonAmount; ++i_Icon)
    {
        DrawIcon(*Buttons[i_Icon], Icons[i_Icon], SetIconColour);
    }
}


void ToolBar::GenerateIconTextures(std::vector<std::string*>& IconStrings, std::vector<Texture2D*>& IconTextures)
{
    //Take in a string of ones and zeroes and turn it into a square texture

    //There's more efficient ways to traverse two linked elements
    //but it's not worth the hassle to rewrite all of this
    for(int i_Button {0}; i_Button < ButtonAmount; ++i_Button)
    {
        std::string IconString = *IconStrings[i_Button];
        Texture2D* IconTexture = IconTextures[i_Button];

        //Allocate 4 bytes of memory [RGBA] for each pixel
        unsigned char* RawPixelData = (unsigned char*)RL_MALLOC(IconPixelAmount * 4);

        //Iterate through each pixel
        for(int i_IconString {0}; i_IconString < IconPixelAmount; ++i_IconString)
        {   
            //Get the index of the current 4 byte [RGBA] block
            int i_PixelData = i_IconString * 4;

            //RGB values can be bundled, only the Alpha is relevant
            RawPixelData[i_PixelData]     = RGBValMax; //[R]
            RawPixelData[i_PixelData + 1] = RGBValMax; //[G] 
            RawPixelData[i_PixelData + 2] = RGBValMax; //[B] 

            if(IconString[i_IconString] == '1')
            {
                //Store a visible pixel [A]
                RawPixelData[i_PixelData + 3] = RGBValMax;
            }
            else
            {
                //Store an invisible pixel [A]
                RawPixelData[i_PixelData + 3] = RGBValMin;
            }
        }

        //The binary string has now been converted to a raw pixel data string
        //Time to load it as a texture
        Image ConvertedIcon = {
            .data = RawPixelData,
            .width = IconPixelDimension,
            .height = IconPixelDimension,
            .mipmaps = 1,
            .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
                              };
        *IconTexture = LoadTextureFromImage(ConvertedIcon);
        UnloadImage(ConvertedIcon);
    }
}


void ToolBar::DrawIcon(Rectangle& ButtonArea, Texture2D* IconImage, Color& IconColour)
{
    //Draw an icon on top of the toolbar button

    //Scale the icon down to the smallest side
    float SmallestSide = ((ButtonArea.width >= ButtonArea.height) * ButtonArea.height) + ((ButtonArea.width< ButtonArea.height) * ButtonArea.width);
    float ScaleIcon = SmallestSide / (IconPixelDimension * 1.25); 

    //Get offsets so the icon always sits in the center
    float OffsetX = (ButtonArea.width  / 2) - ((IconImage->width  * ScaleIcon) / 2);
    float OffsetY = (ButtonArea.height / 2) - ((IconImage->height * ScaleIcon) / 2);

    DrawTextureEx(*IconImage, {ButtonArea.x + OffsetX, ButtonArea.y + OffsetY}, 0, ScaleIcon, IconColour);
}
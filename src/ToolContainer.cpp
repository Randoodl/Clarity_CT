#include "../include/ToolContainer.h"


ToolContainer::ToolContainer(char*& PassedBinPath)
{  
    //Grab executable path
    BinPath = PassedBinPath;

    //Initialise all colours in the collection.
    ColourCollection.Update();

    //Initialise Frame related data
    FrameIsMutable = false;
    Interactions.R_FrameState = FrameIsMutable;

    //Set UI options (might be overwritten by loading the .conf)
    DarkModeEnabled = true;
    HexModeEnabled = true;

    //Set aside all the memory addresses of the Frames object etc. to refer to throughout the program
    //    NOTE: MouseClickHandler interacts with the first Frame it can find in ElementFrames, this means: 
    //    -ToolBar has to come first as to always be accessible
    //    -RGBSquareFrame has to come before RGBDial, otherwise clicks within RGBSquareFrame interact with RGBDial instead
    ElementFrames  = {&ToolBarFrame, &RGBSquareFrame, &RGBDialFrame, &BaseHueFrame, &ComplementFrame, &LowerTriadFrame, &UpperTriadFrame,
                      &MainShadesTintsFrame, &ComplementShadesTintsFrame, &LowerTriadShadesTintsFrame, &UpperTriadShadesTintsFrame, &CurrentSelectedColourFrame};
    HiddenFrames   = {&RGBSquareFrame};

    LayoutPositions = {&Layout.TOOL, &Layout.SQUARE, &Layout.DIAL, &Layout.HUE, &Layout.COMP, &Layout.LTRIAD, &Layout.UTRIAD,
                       &Layout.MAINST, &Layout.COMPST, &Layout.LTRIST, &Layout.UTRIST, &Layout.SELECT};

    AllPalettes    = {&Hue, &Complement, &LowerTriad, &UpperTriad, &MainShadesTints, &ComplementShadesTints, &LowerTriadShadesTints, &UpperTriadShadesTints};
    PaletteActions = {
                        {&Hue, {&ColourCollection.BaseHueColour, &ColourCollection.ShadedColour}},
                        {&Complement, {&ColourCollection.ComplementColour, &ColourCollection.ShadedComplementColour}},
                        {&LowerTriad, {&ColourCollection.LowerTriadColour, &ColourCollection.LowerTriadShade}},
                        {&UpperTriad, {&ColourCollection.UpperTriadColour, &ColourCollection.UpperTriadShade}},
                        {&MainShadesTints, {&ColourCollection.ShadedColour}},
                        {&ComplementShadesTints, {&ColourCollection.ShadedComplementColour}},
                        {&LowerTriadShadesTints, {&ColourCollection.LowerTriadShade}},
                        {&UpperTriadShadesTints, {&ColourCollection.UpperTriadShade}}
                     };
    
    //Try and load in the config file
    //If it doesn't exist/is inccessible, it falls back on the values given in Defaults.h
    LoadCustomConfig(BinPath);
    InitialiseAllElements();
    SetUIColours(DarkModeEnabled);
    UpdateWindowMinimumSize();
}


void ToolContainer::DrawElements()
{
    //Simply combining all drawing calls, the order of which determines visibility on screen

    //Draw the single instance elements, ShadeSquare after Dial so the ShadePreviewSquare doesn't hide behind the Dial
    RGBDial.DrawRGBDial(ColourCollection.BackgroundColour);
    RGBSquare.DrawShadeSquare();
    CurrentSelectedColourFrame.DrawSingleColour(ColourCollection.CurrentSelectedColour);
    
    //Draw all Palettes
    for(Palette* EachPalette : AllPalettes)
    {
        EachPalette->DrawPalette();
    }

    //Toolbar has to be the last draw call, it has to ALWAYS be visible
    Tools.DrawToolBar(ColourCollection.ToolBackgroundColour, ColourCollection.ToolButtonColour, ColourCollection.ToolIconColour);  

    //Draw the FrameBoxes around elements when unlocked
    if(FrameIsMutable)
    {
        for(Frames* ShowFrame : ElementFrames)
        {
            if(std::find(HiddenFrames.begin(), HiddenFrames.end(), ShowFrame) == HiddenFrames.end())
            {
                //This is a laborious way of doing it, but it works. It beats having to keep two vectors updated relative to eachother
                ShowFrame->DrawFrameBox(ColourCollection.FrameBoxColour);
            }
        }
    }
}


void ToolContainer::MouseClickHandler()
{
    //Determine what to interact with once the mouse is clicked

    Vector2 MouseXY = GetMousePosition();

    //When the mouse is clicked, each Frames object is checked to see if the MouseXY falls within its area
    //If this is a case, the ActiveFrame bool is set to true for this Frame
    //While the mouse is held down, this IsMousePressed evaluates to false, but the ActiveFrame bool remains true until the mouse button is released
    //Only one Element can be true at a time, since SetElementInteraction exits as soon as a Frames' ActiveFrame is set to true
    if(IsMouseButtonPressed(0))
    {
        SetElementInteraction(MouseXY);
    }
    
    //As soon as a frame passes wherein the mouse button is released, all ActiveFrame bools are set to false
    if(IsMouseButtonReleased(0))
    {
        SetAllInterActionsToFalse();
    }
    
    //This keeps looping every Frame until it hits the first and ONLY active Frame
    for(int IndexOfFrame {0}; IndexOfFrame < int(ElementFrames.size()); ++IndexOfFrame)
    {
        if(ElementFrames[IndexOfFrame]->ActiveFrame)
        {   
            Interactions.R_FrameState =  FrameIsMutable;
            Interactions.PassedMouseXY = MouseXY;
            DecideElementInteraction(IndexOfFrame);
            break;
        }
    }
}


void ToolContainer::InitialiseAllElements()
{
    //Combining all initialisations under one method

    //Toolbar for various utilities
    ToolBarFrame.Update(Layout.TOOL.AnchorX, Layout.TOOL.AnchorY, Layout.TOOL.LenX, Layout.TOOL.LenY);
    Tools.Update(ToolBarFrame.FrameArea);
    SetAllInterActionsToFalse();
    UpdateWindowMinimumSize();

    //Initialise the Colour Dial's Frame and Element
    DialOffsets = {0, 0, 0};
    RGBDialFrame.Update(Layout.DIAL.AnchorX, Layout.DIAL.AnchorY, Layout.DIAL.LenX, Layout.DIAL.LenY);
    RGBDial.Update(RGBDialFrame.FrameArea.x + RGBDialFrame.FrameArea.width/2, 
                   RGBDialFrame.FrameArea.y + RGBDialFrame.FrameArea.height/2, 
                   RGBDialFrame.FrameArea.width/2);

    //Initialise the Square's Frame and Element
    DialOffsets = RGBDial.GetSquareInDialOffsets();
    RGBSquareFrame.Update(DialOffsets.x, DialOffsets.y, DialOffsets.z, DialOffsets.z);
    RGBSquare.Update(RGBSquareFrame.FrameArea);

    //Initialise the colour previews
    //What a goddamn mess, I need to bundle this somehow
    CurrentSelectedColourFrame.Update(Layout.SELECT.AnchorX, Layout.SELECT.AnchorY, Layout.SELECT.LenX, Layout.SELECT.LenY);
    InitialiseColourPreview(Hue, BaseHueFrame, ColourCollection.BaseHueColour, ColourCollection.ShadedColour, Layout.HUE);
    InitialiseColourPreview(Complement, ComplementFrame, ColourCollection.ComplementColour, ColourCollection.ShadedComplementColour, Layout.COMP);
    InitialiseColourPreview(LowerTriad, LowerTriadFrame, ColourCollection.LowerTriadColour, ColourCollection.LowerTriadShade, Layout.LTRIAD);
    InitialiseColourPreview(UpperTriad, UpperTriadFrame, ColourCollection.UpperTriadColour, ColourCollection.UpperTriadShade, Layout.UTRIAD);

    //Initialise the ShadesTints
    //As above, so below, bundle this
    SetVariationAmount = 9;
    SetVariationDelta = 20;
    InitialiseShadesTints(MainShadesTints, MainShadesTintsFrame, ColourCollection.ShadedColour, SetVariationAmount, SetVariationDelta, Layout.MAINST);
    InitialiseShadesTints(ComplementShadesTints, ComplementShadesTintsFrame, ColourCollection.ShadedComplementColour, SetVariationAmount, SetVariationDelta, Layout.COMPST);

    //NOTE: I swapped out .LowerTriadColour and .UpperTriadColour for their respective shades here in order to fix a minor visual bug
    //      when resetting the layout. However, it might be I used the base colours for good reason, so keep an eye on this
    InitialiseShadesTints(LowerTriadShadesTints, LowerTriadShadesTintsFrame, ColourCollection.LowerTriadShade, SetVariationAmount, SetVariationDelta, Layout.LTRIST);
    InitialiseShadesTints(UpperTriadShadesTints, UpperTriadShadesTintsFrame, ColourCollection.UpperTriadShade, SetVariationAmount, SetVariationDelta, Layout.UTRIST);
}


void ToolContainer::SnapFrames()
{
    //Size Frames down to the few elements that have a set relative dimension (i.e. RGBDial cannot be "squished down", but always has to be square)

    //Snap Frame to ToolBar
    ToolBarFrame.Update(ToolBarFrame.FrameArea.x, ToolBarFrame.FrameArea.y, Tools.ButtonContainer.width, Tools.ButtonContainer.height);

    //Snap Frame to RGBDial
    RGBDialFrame.Update(RGBDial.DialOriginXY.x - RGBDial.DialOuterRadius, RGBDial.DialOriginXY.y - RGBDial.DialOuterRadius, 
                        RGBDial.DialOuterRadius * 2, RGBDial.DialOuterRadius * 2);
    UpdateWindowMinimumSize();
}


void ToolContainer::UpdateWindowMinimumSize()
{
    //Make sure the window can't size down to exclude parts of, or entire, Frames
    //NOTE: this will only size-up the window, not size-down

    //Minumum values to update as each Frames object is considered
    int MinWidth {0};
    int MinHeight  {0};

    //Keep trawling through Frames' bottomright points and retain the largest coordinate point
    for(Frames* Frame : ElementFrames)
    {
        if((Frame->FrameArea.x + Frame->FrameArea.width) > MinWidth)
        {
            MinWidth = (Frame->FrameArea.x + Frame->FrameArea.width);
        }
        if((Frame->FrameArea.y + Frame->FrameArea.height) > MinHeight)
        {
            MinHeight = (Frame->FrameArea.y + Frame->FrameArea.height);
        }
    }
    //Then set the max coordinate point as the min window size
    SetWindowMinSize(MinWidth, MinHeight);
}


void ToolContainer::SetUIColours(bool DarkModeEnabled)
{
    //Hard coded colour values for the UI
    //Maybe in the future this can also be chucked in the .conf

    if(DarkModeEnabled)
    {
        ColourCollection.BackgroundColour = (Color){31, 31, 40, 255};
        ColourCollection.ToolBackgroundColour = (Color){55, 55, 64, 255};
        ColourCollection.ToolButtonColour = (Color){65, 65, 74, 255};
        ColourCollection.ToolIconColour = WHITE;
        ColourCollection.FrameBoxColour = (Color){244, 244, 244, 255};
    }
    else //...why would you ever WANT light mode....?
    {
        ColourCollection.BackgroundColour = (Color){255, 245, 245, 255};
        ColourCollection.ToolBackgroundColour = (Color){214, 208, 208, 255};
        ColourCollection.ToolButtonColour = (Color){255, 245, 245, 255};
        ColourCollection.ToolIconColour = BLACK;
        ColourCollection.FrameBoxColour = (Color){72, 72, 72, 255};
    }
}


void ToolContainer::LoadCustomConfig(char*& PassedBinPath)
{
    //Attempts to load the custom .conf file of all Element position and dimension data
    //Imagine if I implemented a proper way of loading custom UI settings
    //I mean I'm not going to, but just imagine

    //Location of the Clarity executable
    std::filesystem::path ExportPath {PassedBinPath};
    ExportPath = ExportPath.parent_path();

    //Check if the .conf file exists, or exit early and fall back to default Layout values
    if(std::filesystem::exists(ExportPath / "Clarity.conf"))
    {
        try
        {
            //Stores all Element parameters as vectors of ints
            std::vector<std::vector<int>> AllParameters;

            //Open the file and create a string for reading lines
            std::ifstream OpenExportFile(ExportPath / "Clarity.conf");
            std::string ElementParameters;

            //Read each line in the .conf
            while(getline(OpenExportFile, ElementParameters))
            {
                std::stringstream ReadString(ElementParameters);
                std::string SingleParameter;

                //stores [x, y, width, height]
                std::vector<int> ParametersPerLine;

                //Read a single line and split it on the ','
                while(getline(ReadString, SingleParameter, ','))
                {
                    //Create the [x, y, width, height] vector for this line
                    ParametersPerLine.emplace_back(stoi(SingleParameter));
                }
                AllParameters.emplace_back(ParametersPerLine);
            }

            //Set a line counter
            int ReadingLine {0};

            //Iterate through each Frame (even the hidden ones)
            for(ElementPosition* ElementUIData: LayoutPositions)
            {   
                //Overwrite the default values for all Elements with loaded .conf values
                {
                    ElementUIData->AnchorX = AllParameters[ReadingLine][0];
                    ElementUIData->AnchorY = AllParameters[ReadingLine][1];
                    ElementUIData->LenX    = AllParameters[ReadingLine][2];
                    ElementUIData->LenY    = AllParameters[ReadingLine][3]; 
                }
                ++ReadingLine;
            }

            //Lasty, toggle Darkmode and HexMode
            //It is still stored as a vector<vector<int>>, so it needs to be accessed though back >and< index
            HexModeEnabled  = AllParameters[AllParameters.size() - 2][0];
            DarkModeEnabled = AllParameters[AllParameters.size() - 1][0];
        }
        catch(...) //Well aware this is bad practice, but this doesn't call for actual error handling, I just need it to reset the .conf
        {
            std::cout << "A problem occured reading the config file, resetting to defaults.\n";
            DefaultFallback();
        }
    }
    else
    {
        std::cout << "No config file found, setting defaults\n";
    }
}


void ToolContainer::DefaultFallback()
{
    //If for whatever reason the .conf file cannot be read, just load up the default values

    //Set the Layout to the Backup values
    Layout = BackupLayout;
    DarkModeEnabled = true;
    HexModeEnabled = true;

    //Re-initialise all elements
    InitialiseAllElements();

    //Overwrite the .conf with the default values, by default DarkMode is enabled because BLEGH white screen and Hexes over Deces
    Interactions.ExportElementPositions(ElementFrames, DarkModeEnabled, HexModeEnabled, BinPath);
}


void ToolContainer::SetElementInteraction(Vector2 MouseXY)
{
    //Cycle through each element currently loaded and compare its Frame area to the CLICKED cursor, sets the Frame of the clicked element to true

    for(Frames* Frame: ElementFrames)
    {
        if(CheckCollisionPointRec(MouseXY, Frame->FrameArea))
        {
            Frame->ActiveFrame = true;

            Frame->MouseOffsetX = Frame->FrameArea.x - MouseXY.x;
            Frame->MouseOffsetY = Frame->FrameArea.y - MouseXY.y;

            //Perhaps we're supposed to interact with the Frame and not the element itself
            if(FrameIsMutable) 
            {
                if(CheckCollisionPointRec(MouseXY, Frame->MoveButton))
                {
                    Frame->IsDragging = true;
                }
                if(CheckCollisionPointRec(MouseXY, Frame->ScaleButton))
                {
                    Frame->IsScaling = true;
                }
            }
            break;
        }
    }
}


void ToolContainer::SetAllInterActionsToFalse()
{
    //Reset all Frames interactions as soon as a mouse-up is detected
    for(Frames* Frame: ElementFrames)
    {
        Frame->ActiveFrame = false;
        Frame->IsDragging = false;
        Frame->IsScaling = false;
        Frame->MouseOffsetX = 0;        
        Frame->MouseOffsetY = 0;        
    }
}


void ToolContainer::DecideElementInteraction(int ActiveElementFrame)
{
    //Use the index of the Frame in ElementFrames to determine what Frame needs to be interacted with
    //NOTE: this means the order of Elements in ElementFrames is linked to the order of cases below
    //      not entirely optimal, but whattayagonnadoboutit, eh?

    switch(ActiveElementFrame)
    {
        case 0: 
            Interactions.InteractWithToolBar(ElementFrames, Tools, DarkModeEnabled, HexModeEnabled, BinPath); 
            
            //Reads the ResetFrame state every time the Toolbar is interacted with, kind of eh.......
            if(Interactions.ResetFrames)
            {
                DefaultFallback();
                Interactions.ResetFrames = false; 
            }

            //This does get called every time a button is pressed, not terrible but not great?
            if(!FrameIsMutable){SnapFrames();} 
            SetUIColours(DarkModeEnabled);
            break;

        case 1:  Interactions.InteractWithShadeSquare(RGBSquareFrame, RGBSquare); break;
        case 2:  Interactions.InteractwithRGBDial(RGBSquareFrame, RGBDialFrame, RGBSquare, RGBDial, DialOffsets); break;
        case 3:  Interactions.InteractWithPalette(BaseHueFrame, Hue); break;
        case 4:  Interactions.InteractWithPalette(ComplementFrame, Complement); break;
        case 5:  Interactions.InteractWithPalette(LowerTriadFrame, LowerTriad); break;
        case 6:  Interactions.InteractWithPalette(UpperTriadFrame, UpperTriad); break;
        case 7:  Interactions.InteractWithPalette(MainShadesTintsFrame, MainShadesTints); break;
        case 8:  Interactions.InteractWithPalette(ComplementShadesTintsFrame, ComplementShadesTints); break;
        case 9:  Interactions.InteractWithPalette(LowerTriadShadesTintsFrame, LowerTriadShadesTints); break;
        case 10: Interactions.InteractWithPalette(UpperTriadShadesTintsFrame, UpperTriadShadesTints); break;
        case 11: Interactions.InteractWithFloodFilledFrame(CurrentSelectedColourFrame, ColourCollection.CurrentSelectedColour, HexModeEnabled); break;
        
        default: break;
    }
}


void ToolContainer::InitialiseColourPreview(Palette& PreviewPalette, Frames& PreviewFrame, Color& Base, Color& Shade, ElementPosition& SetLayout)
{
    //Method to combine all colour preview Frame initialisation

    //Set the Frame
    PreviewFrame.Update(SetLayout.AnchorX, SetLayout.AnchorY, SetLayout.LenX, SetLayout.LenY);

    //Set the Palette contained within the Frame
    PreviewPalette.Update(PreviewFrame.FrameArea, 0, 0); //Not using the Variation properties here, so those are set to 0
    PreviewPalette.SetHueShadePair(Base, Shade);
    PreviewPalette.GeneratePaletteRectangles();
}


void ToolContainer::InitialiseShadesTints(Palette& ViewPalette, Frames& ViewFrame, Color& PassColour, int VariationAmount, int VariationDelta, ElementPosition& SetLayout)
{
    //Method to combine all ShadesTints Frames intialisation

    //Set the Frame
    ViewFrame.Update(SetLayout.AnchorX, SetLayout.AnchorY, SetLayout.LenX, SetLayout.LenY);

    //Set Frame subdivide parameters
    ViewPalette.Update(ViewFrame.FrameArea, VariationAmount, VariationDelta);

    //Generate Palette colours and subdivide into rectangles
    ViewPalette.GenerateShadesTints(PassColour);
    ViewPalette.GeneratePaletteRectangles();
}
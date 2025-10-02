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

#include <string>

#include "./Interactions.h"
#include "./Defaults.h"


class ToolContainer
{
    //This class is meant to hold all of the program's "moving parts"
    //As well as the methods to interact with 'em
    
    public:
        ToolContainer(char*& PassedBinPath);

        void DrawElements();
        void MouseClickHandler();

        //The collection of UI colours
        ColourFamily ColourCollection;

        //Font VCR_OSD_MONO, courtesy of Riciery Santos Leal
        Font BoxFont;

    private:

        //UI related attributes and methods 
        char* BinPath;
        bool FrameIsMutable;  
        bool DarkModeEnabled;
        bool HexModeEnabled;
        void InitialiseAllElements();
        void SnapFrames();
        void UpdateWindowMinimumSize();
        void SetUIColours(bool DarkModeEnabled);
        
        //Two copies of the layout struct, one to overwrite with values from a possible .conf, one to keep as a backup if all else fails
        Defaults Layout;
        const Defaults BackupLayout; 

        //Custom layout methods
        void LoadCustomConfig(char*& PassedBinPath);
        void DefaultFallback();

        //Interactions of elements with the Mouse
        ElementInteractions Interactions = ElementInteractions(FrameIsMutable, ColourCollection, PaletteActions);
        void SetElementInteraction(Vector2 MouseXY);
        void SetAllInterActionsToFalse();
        void DecideElementInteraction(int ActiveElementFrame);

        //Toolbar - button based functionality
        Frames ToolBarFrame;
        ToolBar Tools;

        //RGBSquare - adjust the shade/tint of a selected hue
        Frames RGBSquareFrame;
        Vector3 DialOffsets;
        ShadeSquare RGBSquare;

        //RGBDial - select a hue
        Frames RGBDialFrame;
        ColourDial RGBDial; 

        //Single colour frames - shows related colours and their shade/tint
        Frames BaseHueFrame;
        Palette Hue;
        Frames ComplementFrame;
        Palette Complement;
        Frames LowerTriadFrame;
        Palette LowerTriad;
        Frames UpperTriadFrame;
        Palette UpperTriad;
        void InitialiseColourPreview(Palette& PreviewPalette, Frames& PreviewFrame, Color& Base, Color& Shade, ElementPosition& SetLayout);

        //ShadesTints - generate a whole spectrum of shades and tints, centered on the current colour shade/tint
        int SetVariationAmount;
        int SetVariationDelta;

        Frames MainShadesTintsFrame;
        Palette MainShadesTints;
        Frames ComplementShadesTintsFrame;
        Palette ComplementShadesTints;
        Frames LowerTriadShadesTintsFrame;
        Palette LowerTriadShadesTints;
        Frames UpperTriadShadesTintsFrame;
        Palette UpperTriadShadesTints;
        void InitialiseShadesTints(Palette& ViewPalette, Frames& ViewFrame, Color& PassColour, int VariationAmount, int VariationDelta, ElementPosition& SetLayout);

        //CurrentSelectedColour - show currently narrowed-down colour and retrieve RGB/Hex value 
        Frames CurrentSelectedColourFrame;
        void ShowCurrentValue(bool PassedCodeMode);
 
        //Collections of objects for for loop functionality
        std::vector<Frames*> ElementFrames;
        std::vector<Frames*> HiddenFrames;
        std::vector<ElementPosition*> LayoutPositions;
        std::vector<Palette*> AllPalettes;
        std::map<Palette*, std::vector<Color*>> PaletteActions;
};
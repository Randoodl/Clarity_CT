#pragma once

#include "./ColourDial.h"
#include "./ShadeSquare.h"
#include "./Frames.h"
#include "./Palette.h"
#include "./ToolBar.h"
#include "./ColourFamily.h"

namespace ElementInteractions
{
    //This is an attempt to isolate all interactions in their own file with the intent to declutter ToolContainer
    //The arguments passed to these functions follows the following order:
    //
    //  MouseXY
    //  FrameIsMutable
    //  Frames
    //  Single Elements
    //  Palettes
    //  ColourFamily
    //  Additional arguments
    //
    //Multiple Frames or Elements are passed in order of descending priority (i.e. ToolBarFrame is always passed as a first frame, if present)

    void InteractWithToolBar(Vector2 MouseXY, bool& FrameIsMutable, Frames& ToolBarFrame, ToolBar& Tools);

    void InteractWithShadeSquare(Vector2 MouseXY, bool FrameIsMutable, Frames& RGBSquareFrame, ShadeSquare& RGBSquare, 
                                 Palette& MainShadesTints, ColourFamily& ColourCollection);

    void InteractwithRGBDial(Vector2 MouseXY, bool FrameIsMutable, Frames& RGBSquareFrame, Frames& RGBDialFrame, ShadeSquare& RGBSquare,
                             ColourDial& RGBDial, Palette& MainShadesTints, ColourFamily& ColourCollection, Vector3& DialOffsets);
    
    void InteractWithMainShadesTints(Vector2 MouseXY, bool FrameIsMutable, Frames& MainShadesTintsFrame, Palette& MainShadesTints);
}
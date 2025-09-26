#pragma once


class ElementPosition
{
    //This class holds all the information related to a Frames' position

    public:
        ElementPosition(int SetAnchorX, int SetAnchorY, int SetLenX, int SetLenY);
    
        int AnchorX;
        int AnchorY;
        int LenX;
        int LenY;
};
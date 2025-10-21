#pragma once

#include "main.h"
#include "displayedImage.h"
#include "cursor.h"

class DisplayedImage;
class Cursor;
class SelectionPanel {
    public:
        char getImageInstructions[128];
        char fileNameInput[128];
        char fileName[128];
        char filePath[128];
        char imageList[128];
        Rectangle rectangle;
        Rectangle colorRec1;
        Rectangle colorRec2;
        Rectangle colorRec3;
        Rectangle imageInputRec;
        Rectangle getImageButtonRec;
        Rectangle getImageInstructionsRec;
        Rectangle colorPickInstructionsRec;
        Rectangle reloadPaletteButtonRec;
        Rectangle cropImageButtonRec;
        Rectangle confirmCropButtonRec;
        Rectangle discardCropButtonRec;
        Rectangle revertImageButtonRec;
        Rectangle imageListTextRec;
        Rectangle imageListRec;
        Color pixelColor;
        Color color1;
        Color color2;
        Color color3;
        Color oldColor1;
        Color oldColor2;
        Color oldColor3;
        int color1Freq;
        int color2Freq;
        int color3Freq;
        int imageListScrollIndex;
        int imageListActive;
        bool color1ButtonPressed;
        bool color2ButtonPressed;
        bool color3ButtonPressed;
        bool getImageButtonPressed;
        bool reloadPaletteButtonPressed;
        bool cropImageButtonPressed;
        bool confirmCropButtonPressed;
        bool discardCropButtonPressed;
        bool revertImageButtonPressed;
        bool paletteLoaded;
        bool croppingImage;

        SelectionPanel();
        void updateImageInstructions(DisplayedImage& img);
        void getTopColors(DisplayedImage& img);
        void changeTopColors(DisplayedImage& img, Cursor& mouse);
        void reloadPalette();
};
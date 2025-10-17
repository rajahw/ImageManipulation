#pragma once

#include "raylib.h"
#include <iostream>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <cstring>
#include <cstdint>

inline uint16_t displayPanelWidth = 960;
inline uint16_t displayPanelHeight = 1080;

class SelectionPanel {
    public:
        char getImageInstructions[128];
        char fileNameInput[128];
        char fileName[128];
        char filePath[128];
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

        inline SelectionPanel() {
            strcpy(getImageInstructions, "Type the file name of your image (including png or jpg/jpeg extension)\n\nto retrieve it");
            strcpy(fileNameInput, "");
            strcpy(fileName, "");
            strcpy(filePath, "assets/env.png");

            rectangle = {960.0f, 0.0f, 960.0f, 1080.0f};
            colorRec1 = {984.0f, 776.0f, 288.0f, 280.0f};
            colorRec2 = {1296.0f, 776.0f, 288.0f, 280.0f};
            colorRec3 = {1608.0f, 776.0f, 288.0f, 280.0f};
            imageInputRec = {984.0f, 104.0f, 912.0f, 96.0f};
            getImageButtonRec = {984.0f, 216.0f, 912.0f, 96.0f};
            getImageInstructionsRec = {984.0f, 24.0f, 912.0f, 96.0f};
            colorPickInstructionsRec = {984.0f, 696.0f, 912.0f, 96.0f};
            reloadPaletteButtonRec = {984.0f, 328.0f, 912.0f, 96.0};
            cropImageButtonRec = {984.0f, 440.0f, 912.0f, 96.0f};
            confirmCropButtonRec = {984.0f, 440.0f, 448.0f, 96.0f};
            discardCropButtonRec = {1448.0f, 440.0f, 448.0f, 96.0f};
            revertImageButtonRec = {984.0f, 552.0f, 912.0f, 96.0f};

            pixelColor = BLACK;
            color1 = BLACK;
            color2 = BLACK;
            color3 = BLACK;

            color1ButtonPressed = false;
            color2ButtonPressed = false;
            color3ButtonPressed = false;
            getImageButtonPressed = false;
            reloadPaletteButtonPressed = false;
            cropImageButtonPressed = false;
            confirmCropButtonPressed = false;
            discardCropButtonPressed = false;
            revertImageButtonPressed = false;
            paletteLoaded = false;
            croppingImage = false;
        }
};

class DisplayedImage {
    public:
        Image image;
        Image oldImage;
        Texture2D texture;
        Rectangle rectangle;
};

class Cursor {
    public:
        Vector2 position;
        int selectedRec;
        Rectangle croppedRec;
        Vector2 initialCropPoint;
        Vector2 cropPoint;
        bool initialPointSelected;

        inline Cursor() {
            position = {};
            selectedRec = 0;
            croppedRec = {};
            initialCropPoint = {};
            cropPoint = {};
            initialPointSelected = false;
        }
};
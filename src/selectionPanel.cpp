#include "selectionPanel.h"
#include "displayedImage.h"
#include "cursor.h"

SelectionPanel::SelectionPanel() {
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
    reloadPaletteButtonRec = {984.0f, 328.0f, 912.0f, 96.0f};
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

void SelectionPanel::updateImageInstructions(DisplayedImage& img) {
    if (IsFileNameValid(fileNameInput)) {
        if (IsFileExtension(fileNameInput, ".png")
        || IsFileExtension(fileNameInput, ".jpg")
        || IsFileExtension(fileNameInput, ".jpeg")) { //if extension is valid
            strcpy(fileName, fileNameInput);

            strcpy(filePath, "assets/");
            strcat(filePath, fileName);

            if (FileExists(filePath)) {
                strcpy(getImageInstructions, "Type the file name of your image (including png or jpg/jpeg\n\nextension) to retrieve it");
            }

        } else { //if extension is not valid
            strcpy(getImageInstructions, "Choose a file with a proper extension (png or jpg/jpeg)");
        }
                
    } else { //if extension is not valid
        strcpy(getImageInstructions, "Choose a file with a proper extension (png or jpg/jpeg)");
    }
}

void SelectionPanel::getTopColors(DisplayedImage& img) {
    if (!IsImageValid(img.image) || img.image.width == 0 || img.image.height == 0) {return;}
            
    Color pixelColor;
    int pixelInt;
    std::unordered_map<int, int> colors;
    std::priority_queue<std::pair<int, int>> topColors;

    for (int i = 0; i < img.image.width; ++i) {
        for (int j = 0; j < img.image.height; ++j) {
            pixelColor = GetImageColor(img.image, i, j);
                    
            if (pixelColor.a != 0) {
                pixelInt = ColorToInt(pixelColor);

                auto iter = std::find_if(colors.begin(), colors.end(), [pixelColor](const std::pair<const int, int>& comparedPair) {
                    Color comparedColor = GetColor(comparedPair.first);
                    return (std::abs(comparedColor.r - pixelColor.r) <= 16 && std::abs(comparedColor.g - pixelColor.g) <= 16 && std::abs(comparedColor.b - pixelColor.b) <= 16);
                });

                if (iter != colors.end()) {
                    ++iter->second;
                } else {
                    ++colors[pixelInt];
                }
            }
        }
    }

    for (auto pair : colors) {
        topColors.push(std::make_pair(pair.second, pair.first));
    }
            
    color1 = GetColor(topColors.top().second);
    color1.a = 255;
    color1Freq = topColors.top().first;
    topColors.pop();
    color2 = GetColor(topColors.top().second);
    color2.a = 255;
    color2Freq = topColors.top().first;
    topColors.pop();
    color3 = GetColor(topColors.top().second);
    color3.a = 255;
    color3Freq = topColors.top().first;

    oldColor1 = color1;
    oldColor2 = color2;
    oldColor3 = color3;
}

void SelectionPanel::changeTopColors(DisplayedImage& img, Cursor& mouse) {
    if (CheckCollisionPointRec(mouse.position, img.rectangle)) {
        pixelColor = GetImageColor(img.image, (mouse.position.x - img.rectangle.x), (mouse.position.y - img.rectangle.y));
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !ColorIsEqual(pixelColor, BLANK)) {
        switch(mouse.selectedRec) {
            case 1:
                color1 = pixelColor;
            break;

            case 2:
                color2 = pixelColor;
            break;

            case 3:
                color3 = pixelColor;
            break;

            default: break;
        }
        mouse.selectedRec = 0;
    }
}
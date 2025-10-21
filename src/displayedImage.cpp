#include "displayedImage.h"
#include "selectionPanel.h"
#include "cursor.h"

void DisplayedImage::load(SelectionPanel& panel) {
    if (!panel.croppingImage) {image = LoadImage(panel.filePath);}
            
    if (IsImageValid(image)) {
        panel.paletteLoaded = false;

        //resize image to fit
        float scale = static_cast<float>(displayPanelWidth) / static_cast<float>(image.width);

        if (image.height * scale > displayPanelHeight) {
            scale = static_cast<float>(displayPanelHeight) / static_cast<float>(image.height);
        }

        ImageResize(&image, static_cast<int>(image.width * scale), static_cast<int>(image.height * scale));
        texture = LoadTextureFromImage(image);
        rectangle = {(960.0f - image.width) / 2.0f, (1080.0f - image.height) / 2.0f, static_cast<float>(image.width), static_cast<float>(image.height)};
    } else {
        strcpy(panel.getImageInstructions, "Image failed to load (Invalid or nonexistent)");
    }
}

void DisplayedImage::getCropRec(SelectionPanel& panel, Cursor& mouse) {
    if (CheckCollisionPointRec(mouse.position, rectangle)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (!mouse.initialPointSelected) { //get initial crop point
                mouse.initialCropPoint = mouse.position;
                mouse.initialPointSelected = true;
            }

            mouse.cropPoint = mouse.position;
            mouse.croppedRec = {mouse.initialCropPoint.x, //x
                                mouse.initialCropPoint.y, //y
                                mouse.cropPoint.x - mouse.initialCropPoint.x, //width
                                mouse.cropPoint.y - mouse.initialCropPoint.y}; //height
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            mouse.initialPointSelected = false; //reset initial crop point
        }
    }
}

void DisplayedImage::confirmCrop(SelectionPanel& panel, Cursor& mouse) {
    oldImage = ImageCopy(image);
    ImageCrop(&image, {mouse.croppedRec.x - rectangle.x, //x
                            mouse.croppedRec.y - rectangle.y, //y
                            mouse.croppedRec.width, //width
                            mouse.croppedRec.height} //height
    );

    load(panel);
            
    panel.croppingImage = false;
    mouse.croppedRec = {};
    mouse.initialCropPoint = {};
    mouse.cropPoint = {};
    mouse.initialPointSelected = false;
}

void DisplayedImage::discardCrop(SelectionPanel& panel, Cursor& mouse) {
    panel.croppingImage = false;
    mouse.croppedRec = {};
    mouse.initialCropPoint = {};
    mouse.cropPoint = {};
    mouse.initialPointSelected = false;
}

void DisplayedImage::revert(SelectionPanel& panel) {
    image = ImageCopy(oldImage);
    load(panel);
}
#include "main.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void uploadImage(DisplayedImage&, SelectionPanel&);
void updateImageInstructions(DisplayedImage&, SelectionPanel&);
void getTopColors(DisplayedImage&, SelectionPanel&);
void changeTopColors(DisplayedImage&, SelectionPanel&, Cursor&);
//void cropImage(DisplayedImage&);
bool coloredButton(Rectangle, Color);

int main() {
    InitWindow(1920, 1080, "Image Manipulation");
    SetTargetFPS(60);

    SelectionPanel panel;
    DisplayedImage img;
    Cursor mouse;
    YesNoPopup cropConfirmation;

    GuiLoadStyle("assets/style_dark.rgs");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 32);

    uploadImage(img, panel);

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

            //Input
            mouse.position = GetMousePosition();

            //Update
            if (!panel.paletteLoaded) {
                getTopColors(img, panel);
                panel.paletteLoaded = true;
            }

            if (panel.getImageButtonPressed) {
                std::cout << "Get Image Button Pressed" << '\n';
                updateImageInstructions(img, panel);
                uploadImage(img, panel);
            }

            if (panel.reloadPaletteButtonPressed) {
                std::cout << "Reload Palette Button Pressed" << '\n';
                
                panel.color1 = panel.oldColor1;
                panel.color2 = panel.oldColor2;
                panel.color3 = panel.oldColor3;
            }

            if (panel.cropImageButtonPressed) {
                std::cout << "Crop Image Button Pressed" << '\n';
                panel.croppingImage = true;
            }

            if (panel.color1ButtonPressed) {
                std::cout << "Color 1 Button Pressed" << '\n';
                mouse.selectedRec = 1;
            }

            if (panel.color2ButtonPressed) {
                std::cout << "Color 2 Button Pressed" << '\n';
                mouse.selectedRec = 2;
            }

            if (panel.color3ButtonPressed) {
                std::cout << "Color 3 Button Pressed" << '\n';
                mouse.selectedRec = 3;
            }
            
            if ((mouse.selectedRec == 1 || mouse.selectedRec == 2 || mouse.selectedRec == 3)) {
                changeTopColors(img, panel, mouse);
            }

            //Draw Image
            DrawTextureV(img.texture, {img.rectangle.x, img.rectangle.y}, WHITE);

            /* Make this work so that the croppedRec doesn't persist
            if (cropping) {
                DrawRectangleRec(mouse.croppedRec, {0, 0, 255, 65});
            }
            */

            //Draw Selection UI
            GuiPanel(panel.rectangle, NULL);
            panel.getImageButtonPressed = GuiButton(panel.getImageButtonRec, "GET IMAGE");
            panel.reloadPaletteButtonPressed = GuiButton(panel.reloadPaletteButtonRec, "RELOAD PALETTE");
            panel.cropImageButtonPressed = GuiButton(panel.cropImageButtonRec, "CROP IMAGE");
            GuiTextBox(panel.imageInputRec, panel.fileNameInput, 128, true);
            panel.color1ButtonPressed = coloredButton(panel.colorRec1, panel.color1);
            panel.color2ButtonPressed = coloredButton(panel.colorRec2, panel.color2);
            panel.color3ButtonPressed = coloredButton(panel.colorRec3, panel.color3);
            GuiLabel(panel.getImageInstructionsRec, panel.getImageInstructions);
            GuiLabel(panel.colorPickInstructionsRec, "To replace a palette color, select a color below, then pick one from\n\nthe image");
        
        EndDrawing();
    }
    
    UnloadImage(img.image);
    UnloadTexture(img.texture);

    CloseWindow();

    return 0;
}

void uploadImage(DisplayedImage& img, SelectionPanel& panel) {
    img.image = LoadImage(panel.filePath);
    
    if (IsImageValid(img.image)) {
        panel.paletteLoaded = false;

        //resize image to fit
        float scale = static_cast<float>(displayPanelWidth) / static_cast<float>(img.image.width);

        if (img.image.height * scale > displayPanelHeight) {
            scale = static_cast<float>(displayPanelHeight) / static_cast<float>(img.image.height);
        }

        ImageResize(&img.image, static_cast<int>(img.image.width * scale), static_cast<int>(img.image.height * scale));
        img.texture = LoadTextureFromImage(img.image);
        img.rectangle = {(960.0f - img.image.width) / 2.0f, (1080.0f - img.image.height) / 2.0f, static_cast<float>(img.image.width), static_cast<float>(img.image.height)};
    } else {
        strcpy(panel.getImageInstructions, "Image failed to load (It may not exist)");
    }
}

void updateImageInstructions(DisplayedImage& img, SelectionPanel& panel) {
    if (IsFileNameValid(panel.fileNameInput)) {
        if (IsFileExtension(panel.fileNameInput, ".png")
        || IsFileExtension(panel.fileNameInput, ".jpg")
        || IsFileExtension(panel.fileNameInput, ".jpeg")) { //if extension is valid
            strcpy(panel.fileName, panel.fileNameInput);

            strcpy(panel.filePath, "assets/");
            strcat(panel.filePath, panel.fileName);

            if (FileExists(panel.filePath)) {
                strcpy(panel.getImageInstructions, "Type the file name of your image (including png or jpg/jpeg\n\nextension) to retrieve it");
            }

        } else { //if extension is not valid
            strcpy(panel.getImageInstructions, "Choose a file with a proper extension (png or jpg/jpeg)");
        }
        
    } else { //if extension is not valid
        strcpy(panel.getImageInstructions, "Choose a file with a proper extension (png or jpg/jpeg)");
    }
}

void getTopColors(DisplayedImage& img, SelectionPanel& panel) {
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
    
    panel.color1 = GetColor(topColors.top().second);
    panel.color1.a = 255;
    panel.color1Freq = topColors.top().first;
    topColors.pop();
    panel.color2 = GetColor(topColors.top().second);
    panel.color2.a = 255;
    panel.color2Freq = topColors.top().first;
    topColors.pop();
    panel.color3 = GetColor(topColors.top().second);
    panel.color3.a = 255;
    panel.color3Freq = topColors.top().first;

    panel.oldColor1 = panel.color1;
    panel.oldColor2 = panel.color2;
    panel.oldColor3 = panel.color3;
}

void changeTopColors(DisplayedImage& img, SelectionPanel& panel, Cursor& mouse) {
    if (CheckCollisionPointRec(mouse.position, img.rectangle)) {
        panel.pixelColor = GetImageColor(img.image, (mouse.position.x - img.rectangle.x), (mouse.position.y - img.rectangle.y));
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !ColorIsEqual(panel.pixelColor, BLANK)) {
        switch(mouse.selectedRec) {
            case 1:
                panel.color1 = panel.pixelColor;
            break;

            case 2:
                panel.color2 = panel.pixelColor;
            break;

            case 3:
                panel.color3 = panel.pixelColor;
            break;

            default: break;
        }
        mouse.selectedRec = 0;
    }
}

//make this work
/*
    void cropImage(DisplayedImage& img, SelectionPanel& panel, Cursor& mouse) {
    if (CheckCollisionPointRec(mouse.position, img.rectangle)) {
                if (cropping == false) {
                    
                } else {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && cropConfirmation.windowClosed == true) {
                    //if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        if (!mouse.initialPointSelected) {
                            mouse.initialCropPoint = mouse.position;
                            mouse.initialPointSelected = true;
                        }

                        mouse.cropPoint = mouse.position;
                        mouse.croppedRec = {mouse.initialCropPoint.x, mouse.initialCropPoint.y, mouse.cropPoint.x - mouse.initialCropPoint.x, mouse.cropPoint.y - mouse.initialCropPoint.y};
                    }

                    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                        cropConfirmation.windowClosed = false;
                    }
                }
            }

            if (cropConfirmation.yesPressed) {
                //duplicate image, assign the duplicate to oldImage, then crop original
                //add revert image button
                img.oldImage = ImageCopy(img.image);
                ImageCrop(&img.image, {img.rectangle.x + mouse.croppedRec.x,
                img.rectangle.y + mouse.croppedRec.y,
                img.rectangle.width + mouse.croppedRec.width,
                img.rectangle.height + mouse.croppedRec.height});
                paletteLoaded = false; 

                cropConfirmation.windowClosed = true;
                cropping = false;
                mouse.croppedRec = {};
                cropConfirmation.yesPressed = false;
            }
            if (cropConfirmation.noPressed) {
                cropConfirmation.windowClosed = true;
                cropping = false;
                mouse.croppedRec = {};
                cropConfirmation.noPressed = false;
            }
            
            if (!cropConfirmation.windowClosed) {
                cropConfirmation.windowClosed = GuiWindowBox(cropConfirmation.windowRec, NULL);
                GuiLabel(cropConfirmation.textRec, "Crop this image?");
                cropConfirmation.yesPressed = GuiButton(cropConfirmation.yesRec, "YES");
                cropConfirmation.noPressed = GuiButton(cropConfirmation.noRec, "NO");
            }
}
*/

//Custom variant of raygui GuiButton()
bool coloredButton(Rectangle bounds, Color color) {
    bool result = false;
    GuiState state = guiState;

    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = true;
        }
    }

    if (state == STATE_NORMAL) GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, BORDER_WIDTH), ColorBrightness(color, -0.5f), color);
    else if (state == STATE_FOCUSED) GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, BORDER_WIDTH), ColorBrightness(color, -0.25f), ColorBrightness(color, 0.25f));
    else if (state == STATE_PRESSED) GuiDrawRectangle(bounds, GuiGetStyle(BUTTON, BORDER_WIDTH), color, ColorBrightness(color, 0.5f));

    if (state == STATE_FOCUSED) GuiTooltip(bounds);

    return result;
}
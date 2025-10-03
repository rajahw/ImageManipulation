#include "main.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

void displayResize(DisplayedImage&);
void getTopColors(DisplayedImage&, SelectionPanel&);
bool coloredButton(Rectangle, Color);

int main() {
    InitWindow(1920, 1080, "Image Processor");
    SetTargetFPS(60);

    SelectionPanel panel;
    DisplayedImage img;
    Cursor mouse;
    YesNoPopup cropConfirmation;
    bool color1ButtonPressed = false;
    bool color2ButtonPressed = false;
    bool color3ButtonPressed = false;
    bool getImageButtonPressed = false;
    bool reloadPaletteButtonPressed = false;
    bool cropImageButtonPressed = false;
    bool statsPrinted = false;
    bool paletteLoaded = false;
    bool cropping = false;
    char getImageInstructions[128] = "Type the file name of your image (including png or jpg/jpeg extension)\n\nto retrieve it";
    char fileNameInput[128] = "";
    char fileName[128]  = "";
    char fileExtension[128] = "";
    char fileString[128] = "assets/jeans.png";
    GuiLoadStyle("assets/style_dark.rgs");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 32);
    img.image = LoadImage(fileString);

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

            //Input
            mouse.position = GetMousePosition();

            //Update
            if (!paletteLoaded) {
                if (IsImageValid(img.image)) {
                    displayResize(img);
                    img.texture = LoadTextureFromImage(img.image);
                    img.rectangle = {(960.0f - img.image.width) / 2.0f, (1080.0f - img.image.height) / 2.0f, static_cast<float>(img.image.width), static_cast<float>(img.image.height)};
                    getTopColors(img, panel);
                    paletteLoaded = true;
                } else {
                    strcpy(getImageInstructions, "Image failed to load");
                    paletteLoaded = true;
                }
            }

            if (!statsPrinted) {
                std::cout << 1 << ": " << panel.color1Freq << '\n';
                std::cout << 2 << ": " << panel.color2Freq << '\n';
                std::cout << 3 << ": " << panel.color3Freq << '\n';
                statsPrinted = true;
            }

            if (getImageButtonPressed) {
                std::cout << "Get Image Button Pressed" << '\n';
                
                if (GetFileExtension(fileNameInput) != nullptr) {
                    strcpy(fileExtension, GetFileExtension(fileNameInput));

                    if (strcmp(fileExtension, ".png") || strcmp(fileExtension, ".jpg") || strcmp(fileExtension, ".jpeg")) { 
                        strcpy(fileName, "assets/");
                        strcat(fileName, fileNameInput);
                        if (FileExists(fileName)) {
                            strcpy(fileString, fileName);
                            strcpy(getImageInstructions, "Type the file name of your image (including png or jpg/jpeg\n\nextension) to retrieve it");
                            img.image = LoadImage(fileString);
                            paletteLoaded = false;
                        } else {
                            strcpy(getImageInstructions, "There is no file with the name you inputted");
                        }
                    } else {
                        strcpy(getImageInstructions, "Choose a file with a proper extension (png or jpg/jpeg)");
                    }
                } else {
                    strcpy(getImageInstructions, "Choose a file with a proper extension (png or jpg/jpeg)");
                }
            }

            if (reloadPaletteButtonPressed) {
                std::cout << "Reload Palette Button Pressed" << '\n';
                
                panel.color1 = panel.oldColor1;
                panel.color2 = panel.oldColor2;
                panel.color3 = panel.oldColor3;
            }

            if (cropImageButtonPressed) {
                std::cout << "Crop Image Button Pressed" << '\n';
                cropping = true;
            }

            if (color1ButtonPressed) {
                std::cout << "Color 1 Button Pressed" << '\n';
                mouse.selectedRec = 1;
            }

            if (color2ButtonPressed) {
                std::cout << "Color 2 Button Pressed" << '\n';
                mouse.selectedRec = 2;
            }

            if (color3ButtonPressed) {
                std::cout << "Color 3 Button Pressed" << '\n';
                mouse.selectedRec = 3;
            }

            if (CheckCollisionPointRec(mouse.position, img.rectangle)) {
                if (!cropping) {
                    panel.pixelColor = GetImageColor(img.image, (mouse.position.x - img.rectangle.x), (mouse.position.y - img.rectangle.y));

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !ColorIsEqual(panel.pixelColor, BLANK)) {
                        switch (mouse.selectedRec) {
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
                    }
                } else {
                    //if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && cropConfirmation.windowClosed) {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
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

            //Draw Image
            DrawTextureV(img.texture, {img.rectangle.x, img.rectangle.y}, WHITE);
            if (cropping) {
                DrawRectangleRec(mouse.croppedRec, {0, 0, 255, 65});
            }

            //Draw Selection UI
            GuiPanel(panel.rectangle, NULL);
            getImageButtonPressed = GuiButton(panel.getImageButtonRec, "GET IMAGE");
            reloadPaletteButtonPressed = GuiButton(panel.reloadPaletteButtonRec, "RELOAD PALETTE");
            cropImageButtonPressed = GuiButton(panel.cropImageButtonRec, "CROP IMAGE");
            GuiTextBox(panel.imagePathRec, fileNameInput, 128, true);
            color1ButtonPressed = coloredButton(panel.colorRec1, panel.color1);
            color2ButtonPressed = coloredButton(panel.colorRec2, panel.color2);
            color3ButtonPressed = coloredButton(panel.colorRec3, panel.color3);
            GuiLabel(panel.getImageInstructionsRec, getImageInstructions);
            GuiLabel(panel.colorPickInstructionsRec, "To replace a palette color, select a color below, then pick one from\n\nthe image");
            
            if (!cropConfirmation.windowClosed) {
                cropConfirmation.windowClosed = GuiWindowBox(cropConfirmation.windowRec, NULL);
                GuiLabel(cropConfirmation.textRec, "Crop this image?");
                cropConfirmation.yesPressed = GuiButton(cropConfirmation.yesRec, "YES");
                cropConfirmation.noPressed = GuiButton(cropConfirmation.noRec, "NO");
            }
            
        EndDrawing();
    }
    
    UnloadImage(img.image);
    UnloadTexture(img.texture);

    CloseWindow();

    return 0;
}

void displayResize(DisplayedImage& img) {
    if (img.image.data == nullptr || img.image.width == 0 || img.image.height == 0) return;

    float scale = static_cast<float>(displayPanelWidth) / static_cast<float>(img.image.width);

    if (img.image.height * scale > displayPanelHeight) {
        scale = static_cast<float>(displayPanelHeight) / static_cast<float>(img.image.height);
    }

    ImageResize(&img.image, static_cast<int>(img.image.width * scale), static_cast<int>(img.image.height * scale));
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
                    iter->second++;
                } else {
                    colors[pixelInt]++;
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
#include "main.h"
#include "selectionPanel.h"
#include "displayedImage.h"
#include "cursor.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

bool coloredButton(Rectangle, Color);

int main() {
    InitWindow(1920, 1080, "Image Manipulation");
    SetTargetFPS(60);

    SelectionPanel panel;
    DisplayedImage img;
    Cursor mouse;

    GuiLoadStyle("assets/style_dark.rgs");
    GuiSetStyle(DEFAULT, TEXT_SIZE, 32);

    img.load(panel);

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(GetColor(GuiGetStyle(DEFAULT, LINE_COLOR)));

            //Input
            mouse.position = GetMousePosition();

            //Update
            if (!panel.paletteLoaded) {
                panel.getTopColors(img);
                panel.paletteLoaded = true;
            }

            if (panel.croppingImage) {
                img.getCropRec(panel, mouse);
            }

            if (panel.getImageButtonPressed) {
                std::cout << "Get Image Button Pressed" << '\n';
                panel.updateImageInstructions(img);
                if (panel.canLoadImage) {img.load(panel);}
            }

            if (panel.reloadPaletteButtonPressed) {
                std::cout << "Reload Palette Button Pressed" << '\n';
                panel.reloadPalette();
            }

            if (panel.cropImageButtonPressed) {
                std::cout << "Crop Image Button Pressed" << '\n';
                panel.croppingImage = true;
                panel.cropImageButtonPressed = false;
                //manually switch state since this button isn't always drawn
            }

            if (panel.revertImageButtonPressed) {
                std::cout << "Revert Image Button Pressed" << '\n';
                img.revert(panel);
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

            if (panel.confirmCropButtonPressed) {
                std::cout << "Confirm Crop Button Pressed" << '\n';
                img.confirmCrop(panel, mouse);
                panel.confirmCropButtonPressed = false;
                //manually switch state since this button isn't always drawn
            }

            if (panel.discardCropButtonPressed) {
                std::cout << "Discard Crop Button Pressed" << '\n';
                img.discardCrop(panel, mouse);
                panel.discardCropButtonPressed = false;
                //manually switch state since this button isn't always drawn
            }
            
            if ((mouse.selectedRec == 1 || mouse.selectedRec == 2 || mouse.selectedRec == 3)) {
                panel.changeTopColors(img, mouse);
            }

            if (panel.activeImage != panel.imageListActive) {
                panel.switchImage();
                img.load(panel);
            }

            //Draw Image
            DrawTextureV(img.texture, {img.rectangle.x, img.rectangle.y}, WHITE);

            if (panel.croppingImage) {
                DrawRectangleRec(mouse.croppedRec, {255, 255, 255, 65});
            }
 
            //Draw Selection UI
            GuiPanel(panel.rectangle, NULL);
            panel.getImageButtonPressed = GuiButton(panel.getImageButtonRec, "GET IMAGE");
            panel.reloadPaletteButtonPressed = GuiButton(panel.reloadPaletteButtonRec, "RELOAD PALETTE");
            panel.revertImageButtonPressed = GuiButton(panel.revertImageButtonRec, "REVERT IMAGE");
            GuiTextBox(panel.imageInputRec, panel.fileNameInput, 128, true);
            panel.color1ButtonPressed = coloredButton(panel.colorRec1, panel.color1);
            panel.color2ButtonPressed = coloredButton(panel.colorRec2, panel.color2);
            panel.color3ButtonPressed = coloredButton(panel.colorRec3, panel.color3);
            GuiLabel(panel.getImageInstructionsRec, panel.getImageInstructions);
            GuiLabel(panel.colorPickInstructionsRec, "To replace a palette color, select a color below, then pick one from\n\nthe image");
            if (!panel.croppingImage) {
                panel.cropImageButtonPressed = GuiButton(panel.cropImageButtonRec, "CROP IMAGE");
            } else {
                panel.confirmCropButtonPressed = GuiButton(panel.confirmCropButtonRec, "CONFIRM CROP");
                panel.discardCropButtonPressed = GuiButton(panel.discardCropButtonRec, "DISCARD CROP");
            }
            GuiLabel(panel.imageListTextRec, "SAVED IMAGES");
            GuiListView(panel.imageListRec, panel.imageList, &panel.imageListScrollIndex, &panel.imageListActive);
        
        EndDrawing();
    }
    
    UnloadImage(img.image);
    UnloadTexture(img.texture);

    CloseWindow();

    return 0;
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
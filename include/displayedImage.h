#pragma once

#include "main.h"
#include "selectionPanel.h"
#include "cursor.h"

class SelectionPanel;
class Cursor;
class DisplayedImage {
    public:
        Image image;
        Image oldImage;
        Texture2D texture;
        Rectangle rectangle;

        void load(SelectionPanel& panel);
        void getCropRec(SelectionPanel& panel, Cursor& mouse);
        void confirmCrop(SelectionPanel& panel, Cursor& mouse);
        void discardCrop(SelectionPanel& panel, Cursor& mouse);
        void revert(SelectionPanel& panel);
};
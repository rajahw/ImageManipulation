#pragma once

#include "main.h";

class Cursor {
    public:
        Vector2 position;
        int selectedRec;
        Rectangle croppedRec;
        Vector2 initialCropPoint;
        Vector2 cropPoint;
        bool initialPointSelected;

        Cursor() {
            position = {};
            selectedRec = 0;
            croppedRec = {};
            initialCropPoint = {};
            cropPoint = {};
            initialPointSelected = false;
        }
};
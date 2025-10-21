#pragma once

#include "main.h"

class Cursor {
    public:
        Vector2 position;
        int selectedRec;
        Rectangle croppedRec;
        Vector2 initialCropPoint;
        Vector2 cropPoint;
        bool initialPointSelected;

        Cursor();
};
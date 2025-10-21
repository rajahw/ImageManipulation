#include "cursor.h"

Cursor::Cursor() {
    position = {};
    selectedRec = 0;
    croppedRec = {};
    initialCropPoint = {};
    cropPoint = {};
    initialPointSelected = false;
}
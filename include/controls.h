#ifndef CONTROLS_H
#define CONTROLS_H

#include "core.h"

void handleArrowControls(const uint8_t *keyboardState, float *playerDirY, float *playerDirX);
void handleWasdControls(const uint8_t *keyboardState, float *playerDirY, float *playerDirX);
#endif
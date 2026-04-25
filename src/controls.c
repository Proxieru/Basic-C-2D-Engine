#include <stdint.h>
#include <SDL.h>
#include "movement.h"
#include "core.h"

void handleArrowControls(const uint8_t *keyboardState, float *playerDirY, float *playerDirX) {
        if (keyboardState[SDL_SCANCODE_UP]) {
            (*playerDirY)--;
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            (*playerDirY)++;   
        }
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            (*playerDirX)--;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            (*playerDirX)++;
        }
        normalizePlayerDirection(playerDirX, playerDirY);
}

void handleWasdControls(const uint8_t *keyboardState, float *playerDirY, float *playerDirX) {
    if (keyboardState[SDL_SCANCODE_W]) {
            (*playerDirY)--;
        }
        if (keyboardState[SDL_SCANCODE_S]) {
            (*playerDirY)++;   
        }
        if (keyboardState[SDL_SCANCODE_A]) {
            (*playerDirX)--;
        }
        if (keyboardState[SDL_SCANCODE_D]) {
            (*playerDirX)++;
        }
        normalizePlayerDirection(playerDirX, playerDirY);
}
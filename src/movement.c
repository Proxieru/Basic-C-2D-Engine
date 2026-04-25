#include <math.h>
#include "core.h"

void normalizePlayerDirection(float *X, float *Y) {
    float lengthOfDirection = sqrt((*X)*(*X) + (*Y)*(*Y));
    if (lengthOfDirection > 0) {
        *X /= lengthOfDirection;
        *Y /= lengthOfDirection;
    }
}

float applyPlayerFriction(float playerDirX, float playerDirY, float playerFriction, double delta, Player *player) {
    if (playerDirX == 0 && playerDirY == 0) { // Move into math funcs
            player->velX *= 1.0f / (1.0f + playerFriction * delta);
            player->velY *= 1.0f / (1.0f + playerFriction * delta);
        }    
    return sqrt(player->velX*player->velX + player->velY*player->velY);
}

void clampPlayerSpeed(float speed, float maxPlayerSpeed, Player *player) {
    if (speed > maxPlayerSpeed) {
            float scale = maxPlayerSpeed / speed;
            player->velX *= scale;
            player->velY *= scale;
        }
}

void applyPlayerVelocity(Player *player, float playerDirX, float playerDirY, double delta, float playerAcceleration) {
    player->velX += playerDirX * playerAcceleration * delta;
    player->velY += playerDirY * playerAcceleration * delta;
}
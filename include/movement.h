#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "core.h"

void normalizePlayerDirection(float *X, float *Y);
float applyPlayerFriction(float playerDirX, float playerDirY, float playerFriction, double delta, Player *player);
void clampPlayerSpeed(float speed, float maxPlayerSpeed, Player *player);
void applyPlayerVelocity(Player *player, float playerDirX, float playerDirY, double delta, float playerAcceleration);
#endif
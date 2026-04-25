#ifndef ENTITY_H
#define ENTITY_H


#include <SDL.h>
#include <SDL_ttf.h>
#include "core.h"

void renderEntities(int entityDataIndex, int tileSize, Engine engine, GameAssets textures, Entity *entityData, Camera *camera);
#endif
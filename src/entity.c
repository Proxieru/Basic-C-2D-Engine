#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "core.h"

void renderEntities(int entityDataIndex, int tileSize, Engine engine, GameAssets textures, Entity *entityData, Camera *camera) {
    for (int i = 0; i < entityDataIndex; i++) {
            Entity entity = entityData[i];
            SDL_Rect entityTextureCoords = {entity.entityID * tileSize, 16, 16, 16};
            SDL_Rect entityPhysicalTextureCoords = {(entity.x - camera->x) * camera->zoom, (entity.y - camera->y) * camera->zoom, 16, 16};
            SDL_RenderCopy(engine.renderer, textures.enemytilesheet, &entityTextureCoords, &entityPhysicalTextureCoords);
        }
}

#endif
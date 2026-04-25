#ifndef TILE_H
#define TILE_H


#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include "core.h"

void renderTiles(int lengthOfFileData, TileData *mapTiles, int tileWidth, int tileSize, int zoomTileSize, Camera *camera, Engine *engine, GameAssets *textures);
#endif

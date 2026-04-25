#include <SDL.h>
#include <SDL_ttf.h>
#include "core.h"



void renderTiles(int lengthOfFileData, TileData *mapTiles, int tileWidth, int tileSize, int zoomTileSize, Camera *camera, Engine *engine, GameAssets *textures) {
    for (int i = 0; i < lengthOfFileData; i++) {
            TileData tiles = mapTiles[i];
            TileData nextTiles = mapTiles[i+1];
            int TileX = i % tileWidth;
            int TileY = i / tileWidth; // this absolutely SUCKED to port into its own file
            int screenX = (TileX - TileY) * (tileSize / 2);
            int screenY = (TileX + TileY) * (tileSize / 4);
            SDL_Rect tileTextureCoords = {tiles.TileID * tileSize, 0, 16, 16};
            SDL_Rect physicalTileTextureCoords = {(screenX - tileSize - camera->x) * camera->zoom, (screenY - tileSize - camera->y) * camera->zoom, zoomTileSize, zoomTileSize};
            if (tiles.TileID == 3 && nextTiles.Layer == 1) {
                SDL_RenderCopy(engine->renderer, textures->tilesheet, &tileTextureCoords, &physicalTileTextureCoords);
                SDL_Rect tileTextureCoords = {nextTiles.TileID * tileSize, 0, tileSize * camera->zoom, tileSize * camera->zoom};
                SDL_Rect physicalTileTextureCoords = {screenX * tileSize - camera->x, screenY * tileSize - camera->y, 16, 16};
                SDL_RenderCopy(engine->renderer, textures->tilesheet, &tileTextureCoords, &physicalTileTextureCoords);
            } else {
                SDL_RenderCopy(engine->renderer, textures->tilesheet, &tileTextureCoords, &physicalTileTextureCoords);
            }
        }
}
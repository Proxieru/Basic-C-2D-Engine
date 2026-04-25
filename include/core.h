#ifndef CORE_H
#define CORE_H

#include <SDL.h>
#include <SDL_ttf.h>

enum TileType {
    grass = 0,
    dirt = 1,
    water = 2,
    rock = 3
};

typedef struct tileProperties {
    int TileID;
    int Layer;
} TileData;

typedef struct cameraProperties {
    int x, y;
    float zoom;
} Camera;

typedef struct Engine {
    SDL_Window* window;
    SDL_Renderer *renderer;
    SDL_Event event;
    TTF_Font* font;
} Engine;

typedef struct GameAssets {
    SDL_Texture* tilesheet;
    SDL_Texture* playersheet;
    SDL_Texture* enemytilesheet;
} GameAssets;

typedef struct entityProperties {
    int entityID;
    int Health;
    int x, y;
    int data;
} Entity;

typedef struct playerProperties {
    float x, y; // why did I have this as playerX and y previously??
    float velX, velY;
} Player;


#endif
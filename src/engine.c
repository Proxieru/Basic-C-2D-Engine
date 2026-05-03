#include <SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>
#include "movement.h"
#include "controls.h"
#include "tile.h"
#include "entity.h"
#include "core.h"

uint8_t tileDataBuffer; // lotta variables
uint8_t layerDataBuffer;
float playerAcceleration = 2000.0f;
float maxPlayerSpeed = 200.0f;
float playerFriction = 20.0f;
float playerDirX = 0;
float playerDirY = 0;
int lengthOfFileData = 0;
int tileHeight = 0;
int tileBitAmounts = 2;
int tileWidth = 40;
int tileIndex = 0; 
int tileSize = 16;
int countOfMapTiles = 0;
int entityDataIndex = 0;
int devCounterGenerate = 0; // remove later
int controlScheme = 1; // 0 is arrow keys, 1 is wasd 
int zoomTileSize = 0;
bool allowMove = true;


Entity entityData[100];

void createEntity(int entityID, int health, int x, int y) {
    printf("ENGINE: Attempting to create entity...\n");
    if (entityDataIndex > 100) {
        printf("ENGINE: Tried creating a entity after limit is full!\n");
        return;
    }
    Entity entity;
    entityData[entityDataIndex].entityID = entityID;
    entityData[entityDataIndex].Health = health;
    entityData[entityDataIndex].x = x;
    entityData[entityDataIndex].y = y;
    entityDataIndex++;
    printf("ENGINE: Created new entity! Entity Index: %d\n", entityDataIndex);
}

void callEngineError(const char *error) {
        fprintf(stderr, "%s\n", error);
        fflush(stderr);
        SDL_Quit(); // Forgot to make engine exit on error, whoops!
        exit(1);
}

void printTileMap(TileData* map, int length) { // Debug, delete later
    for (int i = 0; i < length; i++) {
        printf("Tile %d: ID=%d Layer=%d\n",
               i,
               map[i].TileID,
               map[i].Layer);
    }
}

double getDeltaTime() {
    static Uint64 last = 0;
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();
    double delta = (double)(now - last) / freq;
    last = now;
    return delta;
}

TileData* createTileMap(int* lengthOfTileMap, char filename[]) {
    enum TileType Tile = 0;
    FILE* FilePointer = fopen(filename,"rb"); // TODO: dynamic map loading, takes arg for it
    if (FilePointer == NULL) {
        return NULL;
    }
    fseek(FilePointer, 0L, SEEK_END);
    int sizeOfTileMap = ftell(FilePointer); // grabbing file size
    fseek(FilePointer, 0L, SEEK_SET);
    lengthOfFileData = sizeOfTileMap / tileBitAmounts;
    TileData* tileMallocPointer = (TileData*)malloc(lengthOfFileData * sizeof(TileData));
    if (tileMallocPointer == NULL) {
        callEngineError("ENGINE ERROR: Couldn't allocate memory for tilemap reading!\n");
        fclose(FilePointer);
        return NULL;
    }
    TileData TileProps;
    while (lengthOfFileData != tileIndex) {
        fread(&tileDataBuffer, sizeof(uint8_t), 1, FilePointer);
        tileMallocPointer[tileIndex].TileID = tileDataBuffer; // eventually sqash fread warnings
        fread(&layerDataBuffer, sizeof(uint8_t), 1, FilePointer);
        tileMallocPointer[tileIndex].Layer = layerDataBuffer;
        countOfMapTiles++;
        tileIndex++;
    }
    fclose(FilePointer);
    return tileMallocPointer;
}

TileData* loadMap(TileData *mapTiles, char mapname[]) {
    if (mapTiles != NULL) {
        free(mapTiles);
    }
    mapTiles = createTileMap(&countOfMapTiles, mapname);
    return mapTiles;
}


Engine EngineStart() { // Returns engine object for interacting with the window or renderer
    Engine engine;
    TTF_Init();
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ENGINE ERROR: SDL Failed on start: %s\n", SDL_GetError());
    }
    engine.window = SDL_CreateWindow("2D C Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);
    engine.font = TTF_OpenFont("DejaVuSans.ttf", 24);
    if (engine.font == NULL) {
        callEngineError("Font failed to load\n");
    }
    return engine; // Returning engine object so we can access SDL later
}

bool touchingSolidTile() { // dirty func for now
    return false;
}

int main() {
    printf("ENGINE: Starting..\n");
    Engine engine = EngineStart(); // grabbing SDL props
    Camera camera; // after we know we inited we get camera
    Player player = {0};
    GameAssets textures;
    player.x = 32;
    player.y = 32;
    camera.x = player.x;
    camera.y = player.y;
    camera.zoom = 1;
    if (camera.zoom == 0 || camera.zoom < 0) {
        camera.zoom = 1; // Make sure camera.zoom doesn't become zero, creates SDL errors
    }
    //camera.zoom = 2.0; testing zoom
    printf("Camera X: %d", camera.x);
    printf("Camera Y: %d", camera.y);
    TileData *mapTiles = NULL;
    mapTiles = loadMap(mapTiles, "maps/grass_plains.bin");
    createEntity(1, 100, 32, 32);
    printTileMap(mapTiles, countOfMapTiles);
    textures.tilesheet = IMG_LoadTexture(engine.renderer, "assets/tilemap.png");
    if (textures.tilesheet == NULL) {
        callEngineError("ENGINE ERROR: Couldn't find tilemap texture file!");
    }
    textures.playersheet = IMG_LoadTexture(engine.renderer, "assets/playersheet.png");
    if (textures.playersheet == NULL) {
        callEngineError("ENGINE ERROR: Couldn't find player texture file!");
    }
    textures.enemytilesheet = IMG_LoadTexture(engine.renderer, "assets/enemytilesheet.png");
    if (textures.enemytilesheet == NULL) {
        callEngineError("ENGINE ERROR: Couldn't find enemy texture file!");
    }
    SDL_Rect playerTextureCoords = {1, 1, 16, 32};
    bool isRunning = true;
    SDL_Color color = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(engine.font, "2D Engine", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(engine.renderer, textSurface);
    SDL_Rect physicalTextCoords = { 50, 50, textSurface->w, textSurface->h };
    while (isRunning) {
        SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, 255);
        SDL_RenderClear(engine.renderer);
        playerDirX = 0;
        playerDirY = 0;
        double delta = getDeltaTime();
        const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
        if (controlScheme == 0) {
            handleArrowControls(keyboardState, &playerDirY, &playerDirX);
        }
        if (controlScheme == 1) {
            handleWasdControls(keyboardState, &playerDirY, &playerDirX);
        }
        float speed = applyPlayerFriction(playerDirX, playerDirY, playerFriction, delta, &player);
        clampPlayerSpeed(speed, maxPlayerSpeed, &player);
        SDL_Rect physicalPlayerCoords = {(player.x - camera.x) * camera.zoom, (player.y - camera.y) * camera.zoom, 16, 32};
        applyPlayerVelocity(&player, playerDirX, playerDirY, delta, playerAcceleration);
        float newPlayerX = player.x + player.velX * delta;
        if (!touchingSolidTile(mapTiles, newPlayerX, player.y)) {
            player.x = newPlayerX;
        } else {
            player.velX = 0; // No velocity for you!
        }
        float newPlayerY = player.y + player.velY * delta;
        if (!touchingSolidTile(mapTiles, newPlayerY, player.x)) {
            player.y = newPlayerY;
        } else {
            player.velY = 0;
        }
        camera.x = player.x - (640 / 4) / camera.zoom;
        camera.y = player.y - (480 / 4) / camera.zoom;
        zoomTileSize = tileSize * camera.zoom;
        renderTiles(lengthOfFileData, mapTiles, tileWidth, tileSize, zoomTileSize, &camera, &engine, &textures);
        renderEntities(entityDataIndex, tileSize, engine, textures, entityData, &camera);
        SDL_RenderCopy(engine.renderer, textures.playersheet, &playerTextureCoords, &physicalPlayerCoords);
        SDL_RenderCopy(engine.renderer, texture, NULL, &physicalTextCoords);
        SDL_RenderPresent(engine.renderer);
            while (SDL_PollEvent(&engine.event)) {
            if (engine.event.type == SDL_QUIT) {
                isRunning = 0;
                printf("ENGINE: Shutting down..\n");
                }
            }
        }
}

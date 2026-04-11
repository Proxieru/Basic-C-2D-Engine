#include <SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

uint8_t tileDataBuffer; // lotta variables
uint8_t layerDataBuffer;
float playerAcceleration = 2000.0f;
float maxPlayerSpeed = 200.0f;
float playerFriction = 20.0f;
float playerDirX = 0;
float playerDirY = 0;
int nextX = 16;
int nextY = 16;
int lengthOfFileData = 0;
int tileHeight = 0;
int tileWidth = 0;
int tileIndex = 0; 
int tileSize = 16;
int countOfMapTiles = 0;
int entityData[100];
int entityDataIndex = 0;
bool allowMove = true;

enum TileType {
    grass = 0,
    dirt = 1,
    water = 2,
    rock = 3
};

typedef struct playerProperties {
    float playerX, playerY;
    float velX, velY;
} Player;

typedef struct tileProperties {
    int TileID;
    int Layer;
} TileData;

typedef struct entityProperties {
    int entityID;
    int Health;
} Entity;


typedef struct Engine {
    SDL_Window* window;
    SDL_Renderer *renderer;
    SDL_Event event;
} Engine;

void createEntity(int entityID, int health, int x, int y) {
    printf("ENGINE: Attempting to create entity...\n");
    if (entityDataIndex > 100) {
        printf("ENGINE: Tried creating a entity after limit is full!\n");
    }
    entityData[entityDataIndex] = entityID;
    entityData[entityDataIndex + 1] = health;
    entityData[entityDataIndex + 2] = x;
    entityData[entityDataIndex + 3] = y;
    entityDataIndex += 4;
    printf("ENGINE: Created new entity! Entity Index: %d\n", entityDataIndex);
}

void callEngineError(const char *error) {
        fprintf(stderr, "%s\n", error);
        fflush(stderr);
        //SDL_Quit();
        //exit(1);
}

void printTileMap(TileData* map, int length) { // Debug, delete later
    for (int i = 0; i < length; i++) {
        printf("Tile %d: ID=%d Layer=%d\n",
               i,
               map[i].TileID,
               map[i].Layer);
    }
}

void normalizePlayerDirection(float *X, float *Y) {
    float lengthOfDirection = sqrt((*X)*(*X) + (*Y)*(*Y));
    if (lengthOfDirection > 0) {
        *X /= lengthOfDirection;
        *Y /= lengthOfDirection;
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

TileData* createTileMap(int* lengthOfTileMap) {
    enum TileType Tile = 0;
    FILE* FilePointer = fopen("test.bin","rb"); // TODO: dynamic map loading, takes arg for it
    if (FilePointer == NULL) {
        return NULL;
    }
    fseek(FilePointer, 0L, SEEK_END);
    int sizeOfTileMap = ftell(FilePointer); // grabbing file size
    fseek(FilePointer, 0L, SEEK_SET);
    lengthOfFileData = sizeOfTileMap / 2;
    TileData* tileMallocPointer = (TileData*)malloc(lengthOfFileData * sizeof(TileData));
    if (tileMallocPointer == NULL) {
        callEngineError("ENGINE ERROR: Couldn't allocate memory for tilemap reading!\n");
        fclose(FilePointer);
        return NULL;
    }
    TileData TileProps;
    while (lengthOfFileData != tileIndex) {
        fread(&tileDataBuffer, sizeof(uint8_t), 1, FilePointer);
        tileMallocPointer[tileIndex].TileID = tileDataBuffer;
        fread(&tileDataBuffer, sizeof(uint8_t), 1, FilePointer);
        tileMallocPointer[tileIndex].Layer = layerDataBuffer;
        countOfMapTiles++;
        tileIndex++;
    }
    fclose(FilePointer);
    return tileMallocPointer;
}

Engine EngineStart() {
    Engine engine; // lotsa saying engine here!
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("ENGINE ERROR: SDL Failed on start: %s\n", SDL_GetError());
    }
    engine.window = SDL_CreateWindow("2D C Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    engine.renderer = SDL_CreateRenderer(engine.window, -1, SDL_RENDERER_ACCELERATED);
    return engine; // Returning engine object so we can access SDL later
}

int main() {
    printf("ENGINE: Starting..\n");
    Engine engine = EngineStart(); // grabbing SDL props
    Player player = {0};
    player.playerX = 10;
    player.playerY = 10;
    TileData* mapTiles = createTileMap(&countOfMapTiles);
    createEntity(1, 100, 0, 0);
    createEntity(2, 100, 16, 16);
    printTileMap(mapTiles, countOfMapTiles);
    SDL_Texture* tilesheet = IMG_LoadTexture(engine.renderer, "tilemap.png");
    if (tilesheet == NULL) {
        callEngineError("ENGINE ERROR: Couldn't find tilemap texture file!");
    }
    SDL_Texture* playersheet = IMG_LoadTexture(engine.renderer, "playersheet.png");
    if (playersheet == NULL) {
        callEngineError("ENGINE ERROR: Couldn't find player texture file!");
    }
    SDL_Texture* enemytilesheet = IMG_LoadTexture(engine.renderer, "enemytilesheet.png");
    if (enemytilesheet == NULL) {
        callEngineError("ENGINE ERROR: Couldn't find enemy texture file!");
    }
    SDL_Rect playerTextureCoords = {1, 1, 16, 16};
    bool isRunning = true;
    while (isRunning) {
        SDL_SetRenderDrawColor(engine.renderer, 0, 0, 0, 255);
        SDL_RenderClear(engine.renderer);
        playerDirX = 0;
        playerDirY = 0;
        double delta = getDeltaTime();
        const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);
        if (keyboardState[SDL_SCANCODE_UP]) {
            playerDirY--;
            normalizePlayerDirection(&playerDirX, &playerDirY);
        }
        if (keyboardState[SDL_SCANCODE_DOWN]) {
            playerDirY++;
            normalizePlayerDirection(&playerDirX, &playerDirY);    
        }
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            playerDirX--;
            normalizePlayerDirection(&playerDirX, &playerDirY);
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            playerDirX++;
            normalizePlayerDirection(&playerDirX, &playerDirY);
        }
        if (playerDirX == 0 && playerDirY == 0) {
            player.velX *= 1.0f / (1.0f + playerFriction * delta);
            player.velY *= 1.0f / (1.0f + playerFriction * delta);
        }
        float speed = sqrt(player.velX*player.velX + player.velY*player.velY);

        if (speed > maxPlayerSpeed) {
            float scale = maxPlayerSpeed / speed;
            player.velX *= scale;
            player.velY *= scale;
        }
        SDL_Rect physicalPlayerCoords = {player.playerX, player.playerY, 16, 16};
        player.velX += playerDirX * playerAcceleration * delta;
        player.velY += playerDirY * playerAcceleration * delta;
        player.playerX += player.velX * delta;
        player.playerY += player.velY * delta;
        for (int i = 0; i < lengthOfFileData; i++) {
            TileData tiles = mapTiles[i];
            int tileX = i % 20;
            int tileY = i / 20;
            SDL_Rect tileTextureCoords = {tiles.TileID * tileSize, 0, 16, 16};
            SDL_Rect physicalTileTextureCoords = {tileX * tileSize, tileY*16, 16, 16};
            SDL_RenderCopy(engine.renderer, tilesheet, &tileTextureCoords, &physicalTileTextureCoords);
        }
        for (int i = 0; i < entityDataIndex; i += 4) {
            SDL_Rect entityTextureCoords = {entityData[i] * tileSize, 16, 16, 16};
            SDL_Rect entityPhysicalTextureCoords = {entityData[i + 2], entityData[i + 3], 16, 16};
            SDL_RenderCopy(engine.renderer, enemytilesheet, &entityTextureCoords, &entityPhysicalTextureCoords);
        }
        SDL_RenderCopy(engine.renderer, playersheet, &playerTextureCoords, &physicalPlayerCoords);
        SDL_RenderPresent(engine.renderer);
            while (SDL_PollEvent(&engine.event)) {
            if (engine.event.type == SDL_QUIT) {
                isRunning = 0;
                printf("ENGINE: Shutting down..\n");
                }
            }
        }
}

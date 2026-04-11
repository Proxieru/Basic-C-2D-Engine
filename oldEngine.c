#include <SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

enum TileType {
    grass = 0,
    dirt = 1,
    water = 2,
    rock = 3
};
char tileReadBuffer[10];
int lengthOfTileData = 0;
int tileIndex = 0;
int tileBase = 10;
int i = 0;
int baseTileSize = 16;
int baseTileMultiplier = 2;
int playerY = 16;
int playerX = 16;
int nextX = 0;
int nextY = 0;
int PlayerVelocity = 3;
int tileHeight;
int tileWidth;
bool allowMove = true;
bool checkForCollisions = true;
typedef struct {
        int TileID;
        int TileX;
        int TileY;
        int Layer;
    } TileData;

int* createTileArray() {
    enum TileType Tile = 0;
    FILE* fptr = fopen("test.txt","r");
    if (fptr == NULL) {
        return NULL;
    }
    fseek(fptr, 0L, SEEK_END);
    long int sizeOfFile = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);
    lengthOfTileData = sizeOfFile / 10;
    int tileData[tileHeight][tileWidth];
    while (lengthOfTileData != tileIndex) {
        fseek(fptr, tileIndex * tileBase, SEEK_SET);
        fscanf(fptr, tileReadBuffer);
        char TileID = tileReadBuffer[0] - '0';
        int TileX = tileReadBuffer[1] - '0';
        int TileY = tileReadBuffer[2] - '0';
        char Layer = tileReadBuffer[9] - '0';
        tileData[tileIndex].TileID = TileID;
        tileData[tileIndex].TileX = TileX;
        tileData[tileIndex].TileY = TileY;
        tileData[tileIndex].Layer = Layer;
        switch (TileID) {
            case grass:
                printf("Found grass tile\n");
                break;
            case dirt:
                printf("Found dirt tile\n");
                break;
            case water:
                printf("Found water tile\n");
                break;
            case rock:
                printf("Found rock tile\n");
                break;
        }
        printf("%s\n", tileReadBuffer);
        tileIndex += 1;
    }
    return tileData;
}

int getPlayerTile(int* tileData, int tileX, int tileY) {
    for (int i = 0; i < lengthOfTileData; i++) {
        if (tileData[i].TileX == tileX && tileData[i].TileY == tileY) {
            return tileData[i].TileID;
        }
    }
    return -1;
}   

int main(int argc, char* argv[]) {
    int isRunning = 1;
    SDL_Event event;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Failed on init: %s\n", SDL_GetError());
        return 1;
    }
    TileData* tileData = createTileArray();
    printf("Tile Data: %d\n", tileData[0]);
    printf("SDL init was a success\n");
    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* tilesheet = IMG_LoadTexture(renderer, "tilemap.png");
    SDL_Texture* playertilesheet = IMG_LoadTexture(renderer, "playersheet.png");
    if (tilesheet == NULL) { // Null checks for textures
        print("ENGINE ERROR: Couldn't find tilemap texture file!")
    }
    if (playertilesheet == NULL) {
        printf("ENGINE ERROR: Couldn't find player texture file!")
    }
    SDL_Rect playerCoords = {playerX, playerY, 16, 16};
    SDL_Rect playerTexture = {1, 1, 16, 16};
    while (isRunning) {
            while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = 0;
                }
            }
        

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);
        const Uint8 *keyboardState =  SDL_GetKeyboardState(NULL);
        nextX = playerX;
        nextY = playerY;
        allowMove = true;
        checkForCollisions = true;
        if (keyboardState[SDL_SCANCODE_UP]) { // TODO: put into enum if possible
                nextY -= PlayerVelocity;
            }
            if (keyboardState[SDL_SCANCODE_DOWN]) {
                nextY += PlayerVelocity;
            }
            if (keyboardState[SDL_SCANCODE_LEFT]) {
                nextX -= PlayerVelocity;
            }
            if (keyboardState[SDL_SCANCODE_RIGHT]) {
                nextX += PlayerVelocity;
            }
            SDL_Rect playerNextRect = {nextX, nextY, 16, 16};
            allowMove = true;
            for (int i = 0; i < lengthOfTileData; i++) {
                if (tileData[i].TileID == water || tileData[i].TileID == rock) {
                    SDL_Rect tileRect = {tileData[i].TileX * 16, tileData[i].TileY * 16, 16, 16};
                    if (SDL_HasIntersection(&playerNextRect, &tileRect)) {
                        allowMove = false;
                        break;
                    }
                }
            }

        for (int i = 0; i < lengthOfTileData; i++) { // Render loop wooo
            TileData tiles = tileData[i];
            SDL_Rect tileTexture = {tiles.TileID * baseTileSize, 0, 16, 16};
            SDL_Rect tileCoord = {tiles.TileX * baseTileSize, tiles.TileY*16, 16, 16};
            playerCoords.x = nextX;
            playerCoords.y = nextY;
            int tileX = nextX / 16; // Collision system was ass to code literal cancer
            int tileY = nextY / 16;
            int currentPlayerTile = getPlayerTile(tileData, tileX, tileY);
            if (allowMove) {
                playerX = nextX;
                playerY = nextY;
            }
            SDL_RenderCopy(renderer, tilesheet, &tileTexture, &tileCoord);
        }
        SDL_RenderCopy(renderer, playertilesheet, &playerTexture, &playerCoords);
    if (window == NULL) {
        printf("Window couldn't be created: %s\n", SDL_GetError());
    } else {
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
        }
    }    
}
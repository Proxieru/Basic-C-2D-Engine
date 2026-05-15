#include "core.h"
#include <SDL.h>

void renderText(int textDataIndex, Engine engine, GameAssets textures, Text *textData, Camera *camera)
{
    SDL_Color color = {255, 255, 255};
    for (int i = 0; i < textDataIndex; i++)
    {
        Text text = textData[i];
        SDL_Surface *textSurface = TTF_RenderText_Solid(engine.font, text.textContent, color);
        if (!textSurface)
        {
            continue;
        }
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(engine.renderer, textSurface);
        SDL_Rect rect = {text.x, text.y, textSurface->w, textSurface->h};
        SDL_RenderCopy(engine.renderer, textTexture, NULL, &rect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

int deleteText(int textDataIndex, Text *textData)
{
    if (textDataIndex <= 0)
    {
        return 0;
    }

    textDataIndex--;
    textData[textDataIndex].x = 0;
    textData[textDataIndex].y = 0;
    textData[textDataIndex].textContent[0] = '\0';

    printf("Deleted text. New index: %d\n", textDataIndex);

    return textDataIndex;
}
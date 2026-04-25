gcc src/*.c -Iinclude -o engine -g \
    $(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_ttf -lm
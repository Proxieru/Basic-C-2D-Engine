gcc -O2 -fstack-protector-strong -D_FORTIFY_SOURCE=2 \
    engine.c -o engine \
    $(sdl2-config --cflags --libs) -lSDL2_image -lm
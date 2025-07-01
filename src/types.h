#ifndef TYPES_H
#define TYPES_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>


typedef struct nave {
    int x;
    int y;
    ALLEGRO_BITMAP img;
} Nave;

#endif
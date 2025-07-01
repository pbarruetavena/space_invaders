#include "init.h"
#include "types.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

const float FPS = 100;  

const int SCREEN_W = 960;
const int SCREEN_H = 540;

void draw_scenario() {
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(0, 300, 300, 600, al_map_rgb(0, 245, 0));
}

ALLEGRO_BITMAP *nave_img = NULL;

void draw_nave(Nave *nave) {
	nave_img = al_load_bitmap("img/nave-espacial.png");
	if (!nave_img) {
		fprintf(stderr, "Erro ao carregar a imagem da nave!\n");
	}
	al_draw_bitmap(nave_img, nave->x, nave->y, 0);
}

*Nave load_nave() {
    Nave nave;
}

void destroy_nave(Nave *nave) {

    al_destroy_bitmap(nave->img);
}
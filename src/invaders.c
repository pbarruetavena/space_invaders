#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <math.h>


const float FPS = 100;  

const int SCREEN_W = 960;
const int SCREEN_H = 540;
const int NAVE_SPEED = 2;
const int TIRO_SPEED = 7;
const int ALIEN_SPEED = 1;
const int QUANT_ALIENS = 4*5;

typedef struct tiro {
	int x;
	int y;
	int ativo;
	int raio;
} Tiro;

typedef struct nave {
    int x;
    int y;
    ALLEGRO_BITMAP *img;
	int tam;
	Tiro tiro;
} Nave;

typedef struct alien {
	int x;
	int y;
	int tam;
	ALLEGRO_BITMAP *img;
	int ativo;
} Alien;

ALLEGRO_BITMAP* load_scenario() {
	ALLEGRO_BITMAP *fundo = al_load_bitmap("img/fundo.png");
	if (!fundo) {
		fprintf(stderr, "Erro ao carregar imagem de fundo!\n");
		return NULL;
	}
	return fundo;
}

void draw_scenario(ALLEGRO_BITMAP *fundo) {
	al_draw_scaled_bitmap(fundo,
		0, 0,
		al_get_bitmap_width(fundo),
		al_get_bitmap_height(fundo),
		0, 0,
		SCREEN_W, SCREEN_H,
		0);
}

void destroy_scenario(ALLEGRO_BITMAP *fundo) {
	al_destroy_bitmap(fundo);
}


void draw_nave(Nave *nave) {
	al_draw_scaled_bitmap(nave->img,
		0, 0,
		al_get_bitmap_width(nave->img),
		al_get_bitmap_height(nave->img),
		nave->x, nave->y,
		nave->tam, nave->tam,
		0);
	if(nave->tiro.ativo) {
		al_draw_filled_circle(nave->tiro.x, nave->tiro.y, nave->tiro.raio, al_map_rgb(0, 0, 0));
	}
}

Nave* load_nave() {
    Nave *nave = (Nave*) malloc(sizeof(Nave));
	nave->x = SCREEN_W/2;
	nave->y = SCREEN_H - 70;
	nave->tam = 60;
	nave->tiro.ativo = 0;
	nave->tiro.raio = 4;
	ALLEGRO_BITMAP *nave_img = NULL;
	nave_img = al_load_bitmap("img/nave-espacial.png");
	if (!nave_img) {
		fprintf(stderr, "Erro ao carregar a imagem da nave!\n");
	}
	nave->img = nave_img;
	draw_nave(nave);
	return nave;
}

void destroy_nave(Nave *nave) {
    al_destroy_bitmap(nave->img);
	free(nave);
}

Alien* load_all_aliens() {
	ALLEGRO_BITMAP *img = al_load_bitmap("img/alien.png");
	if(!(img)) {
		fprintf(stderr, "Erro ao carregar a imagem do alien!\n");
	}
	int x, y = -30;
	Alien *m = (Alien*) malloc(QUANT_ALIENS * sizeof(Alien));
	if(m == NULL) {
		fprintf(stderr, "Erro ao alocar aliens\n");
	}

	for(int i = 0; i < 4; i++) {
		y+=80;
		x = -40;
		for(int k = 0; k < 5; k++) {
			x+=100;
			m[5*i + k].img = img;
			m[5*i + k].x = x;
			m[5*i + k].y = y;
			m[5*i + k].tam = 60;
			m[5*i + k].ativo = 1;
		}
	}
	return m;
}

void draw_alien(Alien *alien) {
	if(!(alien->ativo)) return;
	if(alien == NULL || alien->img == NULL) {
		fprintf(stderr, "Erro: Alien ou imagem do alien nulos ao tentar desenhar.\n");
        return;
	}
	int meio = alien->tam / 2;
    int draw_x = alien->x - meio;
    int draw_y = alien->y - meio;
	al_draw_scaled_bitmap(alien->img,
		0, 0,
		al_get_bitmap_width(alien->img),
		al_get_bitmap_height(alien->img),
		draw_x, draw_y,
		alien->tam, alien->tam,
		0);
}

void draw_all_alient(Alien *alien) {
	for(int i = 0; i < 20; i++) {
		draw_alien(&alien[i]);
	}
}


void move_direita(Nave *nave) {
	if((nave->x + nave->tam) > SCREEN_W) return;
	nave->x += NAVE_SPEED;
}

void move_esquerda(Nave *nave) {
	if(nave->x < -2) return;
	nave->x -= NAVE_SPEED;
}

void atirar(Nave *nave) {
	if(!(nave->tiro.ativo)) {
		nave->tiro.y = nave->y;
		nave->tiro.x = nave->x + nave->tam/2;
		nave->tiro.ativo = 1;
	}
}

void move_tiro(Tiro *tiro) {
	if(tiro->ativo) {
		tiro->y -= TIRO_SPEED;
	}
}

void get_down_aliens(Alien *aliens) {
	printf("teste\n");
	for(int i = 0; i < QUANT_ALIENS; i++) {
		aliens[i].y += 30;
	}
}

void move_aliens(Alien *alien, int *dir) {
	int aux = 0;
	for(int i = 0; i < QUANT_ALIENS; i++) {
		if(!(alien[i].ativo)) continue;
		if((alien[i].x + alien[i].tam > SCREEN_W && (*dir)==1) ||
			(alien[i].x - alien[i].tam < 0 && (*dir)==-1 )) {
			aux = 1;
		}
		alien[i].x += (*dir) * ALIEN_SPEED;
	}
	if(aux) {
		*dir *= -1;
		get_down_aliens(alien);
	}
}

void _colisao_tiro(Alien *alien, Tiro *tiro, int *pont) {
	if(!(tiro->ativo) || !(alien->ativo)) return;
	double dist = sqrt(pow(alien->x - tiro->x, 2) + pow(alien->y - tiro->y, 2));
	double raios = alien->tam/2 + tiro->raio;
	if(dist <= raios) {
		tiro->ativo = 0;
		alien->ativo = 0;
		(*pont)++;
	}
	if(tiro->y <= 0) {
		tiro->ativo = 0;
	}
}

void verificar_tiro(Alien *alien, Tiro *tiro, int *pont) {
	for(int i = 0; i < QUANT_ALIENS; i++) {
		_colisao_tiro(&alien[i], tiro, pont);
	}
}

int main(int argc, char **argv){
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//inicializa o modulo que permite carregar imagens no jogo
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//instala o mouse
	if(!al_install_mouse()) {
		fprintf(stderr, "failed to initialize mouse!\n");
		return -1;
	}

	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
   


	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//registra na fila os eventos de mouse (ex: clicar em um botao do mouse)
	al_register_event_source(event_queue, al_get_mouse_event_source());  	


	// elementos

	ALLEGRO_BITMAP *img_fundo = load_scenario();
	Nave *nave = load_nave();
	Alien *aliens = load_all_aliens();
	int pontuacao = 0;
	int direcao = 1;


	//inicia o temporizador
	al_start_timer(timer);
	
	int playing = 1;
	while(playing) {
		ALLEGRO_EVENT ev;
		//espera por um evento e o armazena na variavel de evento ev
		al_wait_for_event(event_queue, &ev);

		//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
		if(ev.type == ALLEGRO_EVENT_TIMER) {
			ALLEGRO_KEYBOARD_STATE state;
			al_get_keyboard_state(&state);
			if (al_key_down(&state, ALLEGRO_KEY_RIGHT) || al_key_down(&state, ALLEGRO_KEY_D)) {
				move_direita(nave);
			}
			if (al_key_down(&state, ALLEGRO_KEY_LEFT) || al_key_down(&state, ALLEGRO_KEY_A)) {
				move_esquerda(nave);
			}

			move_tiro(&(nave->tiro));
			move_aliens(aliens, &direcao);
			verificar_tiro(aliens, &(nave->tiro), &pontuacao);

            draw_scenario(img_fundo);
			draw_nave(nave);
			draw_all_alient(aliens);
			
			//atualiza a tela (quando houver algo para mostrar)
			al_flip_display();
			
			if(al_get_timer_count(timer)%(int)FPS == 0)
				printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
		}
		//se o tipo de evento for o fechamento da tela (clique no x da janela)
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			playing = 0;
		}
		//se o tipo de evento for um clique de mouse
		else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			printf("\nmouse clicado em: %d, %d", ev.mouse.x, ev.mouse.y);
		}
		//se o tipo de evento for um pressionar de uma tecla
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//imprime qual tecla foi
			printf("\ncodigo tecla: %d", ev.keyboard.keycode);
			switch(ev.keyboard.keycode) {
				case ALLEGRO_KEY_RIGHT:
				case ALLEGRO_KEY_D:
					move_direita(nave);
					break;

				case ALLEGRO_KEY_LEFT:
				case ALLEGRO_KEY_A:
					move_esquerda(nave);
					break;

				case ALLEGRO_KEY_SPACE:
					atirar(nave);
					break;
			}
		}

	} //fim do while
     
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	destroy_scenario(img_fundo);
	destroy_nave(nave);
 
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
   
 
	return 0;
}
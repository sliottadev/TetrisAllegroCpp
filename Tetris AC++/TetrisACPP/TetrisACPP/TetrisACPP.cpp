#include "allegro5/allegro.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_image.h"
#include <iostream>
#include "constants.h"
#include "enumerated.h"
#include "Piece.h"
#include "LogicalGame.h"
#include "TextHelper.h"
#include <chrono>
#include <sstream>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

void drawActualPiece(ALLEGRO_DISPLAY* display, Piece* actualPiece, ALLEGRO_BITMAP* bmp);
void drawNextPiece(ALLEGRO_DISPLAY* display, Piece* nextPiece, ALLEGRO_BITMAP* bmp);
long getTimeNow();

void assignNewPiece(Piece* p);
void verifyLeves(int*, int*, int*);
void drawMatrix(LogicalGame* matrix, ALLEGRO_BITMAP* bmp, int* lineas, int* puntaje, int* level, int*);

int getRandomPiece();
int getRandomColor();

int main(){
	srand((unsigned)time(0));
	enum GAME_STATE { GAME_MENU = 0, GAME_IDLE = 1, GAME_PAUSE = 2, GAME_GAMEOVER = 3 };
	
	ALLEGRO_DISPLAY* display;

	if (!al_init()) {
		al_show_native_message_box(NULL, NULL, NULL, "No se pudo Inicializar Allegro 6", NULL, NULL);
		return -1;
	}

	al_set_new_display_flags(ALLEGRO_OPENGL);	
	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);

	al_set_window_title(display, "TETRIS ACPP");
	
	al_install_mouse();
	al_install_keyboard();
	
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_BITMAP* background = al_load_bitmap("res/back.png");
	ALLEGRO_BITMAP* colorBlocks = al_load_bitmap("res/blocks.png");
	ALLEGRO_BITMAP* nextPiecebmp = al_load_bitmap("res/next.png");

	ALLEGRO_KEYBOARD_STATE keyState;
	ALLEGRO_TIMER* timer = al_create_timer(1.0f / GAME_FPS);
	ALLEGRO_EVENT_QUEUE* eventQueque = al_create_event_queue();
	ALLEGRO_FONT* font = al_load_font("fonts/score.ttf", 40, NULL);

	al_register_event_source(eventQueque, al_get_keyboard_event_source());
	al_register_event_source(eventQueque, al_get_timer_event_source(timer));
	al_register_event_source(eventQueque, al_get_display_event_source(display));
	al_register_event_source(eventQueque, al_get_mouse_event_source());

	//al_hide_mouse_cursor(display);
	al_start_timer(timer);

	bool done = false;
	bool drawGame = true;
	int speed = 1000; //(1000ms = 1s)

	int score = 0;
	int lines = 0;
	int level = 0;

	LogicalGame* gameMatrix = new LogicalGame(START_GAMEFRAME_X, START_GAMEFRAME_Y);

	GAME_STATE gameState = GAME_IDLE;

	Piece* actualPiece = new Piece(getRandomPiece());
	Piece* nextPiece = new Piece(getRandomPiece());
	
	long timeTick = getTimeNow();
	while (!done) {
		ALLEGRO_EVENT events;
		al_wait_for_event(eventQueque, &events);

		if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				if (gameState == GAME_IDLE)
					gameState = GAME_PAUSE;
				else
					gameState = GAME_IDLE;
			}
		}

		if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			done = true;
		}

		if (events.type == ALLEGRO_EVENT_KEY_DOWN) {
			al_get_keyboard_state(&keyState);
			if (al_key_down(&keyState, ALLEGRO_KEY_LEFT)) 
				actualPiece->moveLeft();
			if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
				actualPiece->moveRight();
			if (al_key_down(&keyState, ALLEGRO_KEY_DOWN))
				actualPiece->moveVertical();
			if (al_key_down(&keyState, ALLEGRO_KEY_UP))
				actualPiece->rotateLeft();
			if (al_key_down(&keyState, ALLEGRO_KEY_Z))
				actualPiece->rotateRight();
			if (al_key_down(&keyState, ALLEGRO_KEY_X))
				actualPiece->rotateLeft();
		}

		if (events.type == ALLEGRO_EVENT_TIMER) {
			long nowt = getTimeNow();
			if (gameState == GAME_IDLE){
				if ((timeTick - getTimeNow()) <= -speed) {
					actualPiece->moveVertical();
					timeTick = getTimeNow();
				}

				if (gameMatrix->checkColision(actualPiece, 0, 1)) {
					gameMatrix->mergePiece(actualPiece);
					actualPiece = nextPiece;
					nextPiece= new Piece(getRandomPiece());
					gameMatrix->verifyLines();
				}
				al_draw_bitmap(background, 0, 0, NULL);
				drawMatrix(gameMatrix, colorBlocks, &lines, &score, &level, &speed);
				drawActualPiece(display, actualPiece, colorBlocks);
				drawNextPiece(display, nextPiece, nextPiecebmp);
				std::ostringstream xcore;
				std::ostringstream xlevel;
				std::ostringstream xlines;
				xlines << lines;
				xcore << score;
				xlevel << level;
				al_draw_text(font, al_map_rgb(255, 198, 0), 600, 260, ALLEGRO_ALIGN_RIGHT, xcore.str().c_str());
				al_draw_text(font, al_map_rgb(255, 198, 0), 600, 360, ALLEGRO_ALIGN_RIGHT, xlines.str().c_str());
				al_draw_text(font, al_map_rgb(255, 198, 0), 600, 440, ALLEGRO_ALIGN_RIGHT, xlevel.str().c_str());
			}
			std::ostringstream oss;
			oss << "timeTick: " << timeTick << " - tick: " << nowt;
			al_draw_text(font, al_map_rgb(0, 255, 0), 0, 0, ALLEGRO_ALIGN_LEFT, oss.str().c_str());
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
		if (gameState == GAME_PAUSE) {
			al_draw_text(font, al_map_rgb(0, 255, 0), SCREEN_WIDTH  / 2, SCREEN_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "PAUSE");
		}
	}

	
	al_rest(1.0f);
	al_destroy_display(display);
	al_destroy_bitmap(background);
	al_destroy_bitmap(nextPiecebmp);
	al_destroy_bitmap(colorBlocks);
	al_destroy_event_queue(eventQueque);
	al_destroy_font(font);
}

void drawActualPiece(ALLEGRO_DISPLAY* display, Piece* actualPiece, ALLEGRO_BITMAP *bmp) {
	int poX, poY;

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			if (actualPiece->getMatrizValue(col, row)==1){
				poX = START_GAMEFRAME_X + (actualPiece->getXPos() * BLOCK_SIZE ) +(col * BLOCK_SIZE);
				poY = START_GAMEFRAME_Y + (actualPiece->getYpos() * BLOCK_SIZE) + (row * BLOCK_SIZE);
				al_draw_bitmap_region(bmp, actualPiece->getColor() * BLOCK_SIZE, 0, 38, 38, poX, poY, NULL);
			}
		}
	}
}

void drawNextPiece(ALLEGRO_DISPLAY* display, Piece* nextPiece, ALLEGRO_BITMAP* bmp) {
	int poX, poY;
	poX = 520;
	poY = 60;
	al_draw_bitmap_region(bmp, nextPiece->getColor() * 129, 0, 129, 74, poX, poY, NULL);
}

long getTimeNow(){
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void assignNewPiece(Piece* p) {
	p->setNewPiece(getRandomPiece());
}

void verifyLeves(int* lines, int* level, int* speed)
{
	*level = *lines / GAME_LINES_PER_LEVELS;
	*speed = *speed - (*level * GAME_SPEED_JUMP_LEVEL);
}

void drawMatrix(LogicalGame* matrix, ALLEGRO_BITMAP* bmp, int* lineas, int *puntaje, int *level, int* speed) {
	int poX, poY;
	int randColor;
	for (int row = GAME_MATRIX_ROW - 1; row >= 0; row--) {
		randColor = -1;
		if (matrix->isLineComplete(row)) {
			randColor = getRandomColor();
			if (matrix->lineCompleteDownTransitions(row) <= 0) {
				matrix->popLine(row);
				*lineas=*lineas + 1;
				*puntaje = *lineas * GAME_SCORE_PER_LINE;
				verifyLeves(lineas, level, speed);
				matrix->verifyLines();
			}
		}

		for (int col = 0; ((col < GAME_MATRIX_COL)); col++) {
			poX = START_GAMEFRAME_X + (col * BLOCK_SIZE);
			poY = START_GAMEFRAME_Y + (row * BLOCK_SIZE);
			if (randColor != -1)
				al_draw_bitmap_region(bmp, randColor * BLOCK_SIZE, 0, 38, 38, poX, poY, NULL);
			else
				al_draw_bitmap_region(bmp, matrix->getBlockValue(row, col) * BLOCK_SIZE, 0, 38, 38, poX, poY, NULL);
		}

	}
}

int getRandomPiece() {
	return  1 + (rand() % 7);
}

int getRandomColor() {
	return 1 + (rand() % 7);
}
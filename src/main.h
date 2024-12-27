#ifndef MAIN_H
#define MAIN_H

#include "system.h"

enum PaddleDirection { Up=-1, Down=1 };

typedef struct {
  u8 player_one_score;
  u8 player_two_score;
  u8 game_mode;
} GameState;

typedef struct {
  u8 colour;
  u8 sizeX;
  u8 sizeY;
  i16 xCor;
  i16 yCor;
} Paddle;

typedef struct {
  u8 colour;
  u8 size;
  i8 velocityX;
  i8 velocityY;
  i16 xCor;
  i16 yCor;
} Ball;

extern GameState game_state;
extern Paddle left_padle, right_paddle;
extern Ball ball;

void init();
void menu_init();
void game_init();

void draw_paddle(Paddle *paddle, u8 colour);
void draw_ball(Ball *ball);
void draw_score();
void draw_game_arena();

void countdown();
void move_paddle(Paddle *paddle, enum PaddleDirection direction);
void reset_ball();
void on_point_scored();
void update_ball();

void keyboard_handler_menu(u8 key);
void keyboard_handler_game(u8 key);

void render_popup(const char *label, void (*listener_func)(volatile boolean *running));
void render_menu();
void render_game();
void render();

#endif

#include "io.h"
#include "irq.h"
#include "screen.h"
#include "isr.h"
#include "idt.h"
#include "system.h"
#include "timer.h"
#include "font.h"
#include "keyboard.h"
#include "fpu.h"

#define FPS 120

#define PADDLE_VEL_Y 10
#define BALL_VEL_X 2
#define BALL_VEL_Y 2

#define PADDLE_PADDING_X 10

#define PRIMARY_COLOUR 15     // White
#define SECONDARY_COLOUR 23   // Grey ish
                     
#define TILE_SIZE 5

// Menu states
#define MENU_START_ONE_PLAYER 0
#define MENU_START_TWO_PLAYER 1
#define MENU_QUIT 2

// Game modes
#define GAME_MODE_MENU 0
#define GAME_MODE_ONE_PLAYER 1
#define GAME_MODE_TWO_PLAYERS 2

static u16 SCREEN_CENTER_X = SCREEN_WIDTH / 2;
static u16 SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;

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

GameState game_state;
Paddle left_paddle, right_paddle;
Ball ball;

// Ball hits wall collision checks
#define BALL_HIT_LEFT_WALL(ball) ((ball).xCor == 0)
#define BALL_HIT_RIGHT_WALL(ball) ((ball).xCor == (SCREEN_WIDTH - (ball).size))
#define BALL_HIT_TOP_WALL(ball) ((ball).yCor == 0)
#define BALL_HIT_BOTTOM_WALL(ball) ((ball).yCor == (SCREEN_HEIGHT - (ball).size))

// Ball hits paddle collision checks
#define BALL_COLLIDES_WITH_LEFT_PADDLE_X(ball, paddle) \
  (((ball).xCor <= ((paddle).xCor) + (paddle).sizeX))
#define BALL_COLLIDES_WITH_LEFT_PADDLE_Y(ball, paddle) \
  (((ball).yCor >= ((paddle).yCor)) && ((ball).yCor <= ((paddle).yCor) + ((paddle).sizeY)))
#define BALL_COLLIDES_WITH_RIGHT_PADDLE_X(ball, paddle) \
  (((ball).xCor <= ((paddle).xCor) + (paddle).sizeX))
#define BALL_COLLIDES_WITH_RIGHT_PADDLE_Y(ball, paddle) \
  (((ball).yCor >= ((paddle).yCor)) && ((ball).yCor <= ((paddle).yCor) + ((paddle).sizeY)))
  
void reset_ball();
void render();

void init() {
  game_state = (GameState) {
    .game_mode = GAME_MODE_MENU,
    .player_one_score = 0,
    .player_two_score = 0,
  };
}

void menu_init() {
  game_state.player_two_score = 0;
  game_state.player_two_score = 0;
}

void game_init() {
  ball = (Ball){
    .colour = PRIMARY_COLOUR,
    .size = 10,
    .xCor = SCREEN_CENTER_X - (ball.size / 2),
    .yCor = SCREEN_CENTER_Y - (ball.size / 2),
    .velocityX = BALL_VEL_X,
    .velocityY = BALL_VEL_Y };

  left_paddle = (Paddle){
    .colour = PRIMARY_COLOUR,
    .xCor = PADDLE_PADDING_X,
    .yCor = 20,
    .sizeX = 10,
    .sizeY = 40};

  right_paddle = (Paddle){
    .colour = PRIMARY_COLOUR,
    .xCor = SCREEN_WIDTH - PADDLE_PADDING_X - 10, 
    .yCor = 20,
    .sizeX = 10,
    .sizeY = 40};
}

// =======
// Drawing
// =======
void draw_paddle(Paddle *paddle, u8 colour) {
  u8 paddle_height = paddle->sizeY;
  u8 paddle_width = paddle->sizeX;

  u16 paddleY = paddle->yCor;
  u16 paddleX = paddle->xCor;

  for (size_t yCor = paddleY; yCor < paddleY + paddle_height; yCor++) {
    for (size_t xCor = paddleX; xCor < paddleX + paddle_width; xCor++) {
      screen_set(xCor, yCor, colour);
    }
  }
}

void draw_ball(Ball *ball) {
  for (size_t y = ball->yCor; y < ball->yCor + ball->size; y++) {
    for (size_t x = ball->xCor; x < ball->xCor + ball->size; x++) {
      screen_set(x, y, ball->colour);
    }
  }
}

void draw_score() {
  font_char('0' + game_state.player_one_score, SCREEN_CENTER_X - 16, 20, PRIMARY_COLOUR);
  font_char('0' + game_state.player_two_score, SCREEN_CENTER_X + 8, 20, PRIMARY_COLOUR);
}

void draw_game_arena() {
  u16 yCor = 0;

  // Draw center line
  while (yCor < SCREEN_HEIGHT) {
    for (size_t x = SCREEN_CENTER_X - 3; x < SCREEN_CENTER_X + 3; x++) {
      screen_set(x, yCor, SECONDARY_COLOUR);
    }

    yCor++;
    if (yCor % 10 == 0) {
      yCor += 10;
    }
  }
}

void draw_game_arena_border() {
  for (size_t yCor = 0; yCor < SCREEN_HEIGHT; yCor++) {
    for (size_t xCor = 0; xCor < SCREEN_WIDTH; xCor++) {
      screen_set(xCor, yCor, SECONDARY_COLOUR);
    }

    if (yCor == 4) {
      yCor = SCREEN_HEIGHT - 5;
    }
  }
}

// ==========
// Game logic
// ==========
void countdown() {
  // TODO: Move this to the rendering function so that we can use the screen clear and swap
  for (size_t i = 3; i > 0; i--) {
    write_serial_string("TIMER: ");
    write_serial_char('0' + i);
    write_serial_string("\n");

    font_char('0' + i, 100, 100, 4);

    // Wait 1 second
    u64 start_ticks = timer_get();
    while (timer_get() - start_ticks < TIMER_TPS);
  }
}

void move_paddle(Paddle *paddle, enum PaddleDirection direction) {
  paddle->yCor += direction * PADDLE_VEL_Y;

  if (paddle->yCor <= 0) {
    paddle->yCor = 0;
  } else if (paddle->yCor + paddle->sizeY > SCREEN_HEIGHT) {
    paddle->yCor = SCREEN_HEIGHT - paddle->sizeY;
  }
}


void reset_ball() {
  ball = (Ball){
    .colour = PRIMARY_COLOUR,
    .size = 10,
    .xCor = SCREEN_CENTER_X - (ball.size / 2),
    .yCor = SCREEN_CENTER_Y - (ball.size / 2),
    .velocityX = BALL_VEL_X,
    .velocityY = BALL_VEL_Y };
}

void on_point_scored() {
  reset_ball();
  countdown();
}

void update_ball() {
  GameState *game_state_ptr = &game_state;

  ball.xCor += ball.velocityX;
  ball.yCor += ball.velocityY;
  
  // Bounce off top and bottom walls
  if (BALL_HIT_TOP_WALL(ball) || BALL_HIT_BOTTOM_WALL(ball)) {
    ball.velocityY = -ball.velocityY;
  }

  // Calculate offset velocity:
  //  https://gamedev.stackexchange.com/questions/4253/in-pong-how-do-you-calculate-the-balls-direction-when-it-bounces-off-the-paddl 
  if (BALL_HIT_LEFT_WALL(ball)) {
    game_state_ptr->player_two_score += 1;
    reset_ball();
    countdown();
  }

  if (BALL_HIT_RIGHT_WALL(ball)) {
    game_state_ptr->player_one_score += 1;
    reset_ball();
    countdown();
  }

  u16 lPaddleY = left_paddle.yCor;
  u16 lPaddleX = left_paddle.xCor;

  u16 rPaddleY = right_paddle.yCor;
  u16 rPaddleX = right_paddle.xCor;

  u16 ballX = ball.xCor;
  u16 ballY = ball.yCor;

  // Bounce off paddles
  if ((ballX <= lPaddleX + left_paddle.sizeX &&
       ballY >= lPaddleY &&
       ballY <= lPaddleY + left_paddle.sizeY) ||
      (ballX + ball.size >= rPaddleX &&
       ballY >= rPaddleY &&
       ballY <= rPaddleY + right_paddle.sizeY)) {
    ball.velocityX = -ball.velocityX;
  }

  // Reset ball if it goes off-screen
  if (ballX <= 0 || ballX + ball.size >= SCREEN_WIDTH) {
    ball.xCor = 160;
    ball.yCor = 120;

    ball.velocityX = BALL_VEL_X * (ball.velocityX < 0 ? -1 : 1); // Maintain direction
    ball.velocityY = BALL_VEL_Y;
  }
}


// =============
// Key listening
// =============
void keyboard_handler_menu(u8 key) {
  switch (key) {
    case '1':
      game_state.game_mode = GAME_MODE_ONE_PLAYER;
      game_init();
      break;
    case '2':
      game_state.game_mode = GAME_MODE_TWO_PLAYERS;
      game_init();
      break;
  }
}

void keyboard_handler_game(u8 key) {
  switch (key) {
    case 'w':
      move_paddle(&left_paddle, Up);
      break;
    case 's':
      move_paddle(&left_paddle, Down);
      break;
    case 27: // Pressing esc goes back to main menu
      menu_init();
      game_state.game_mode = GAME_MODE_MENU;
    }
}

void keyboard_handler() {
  u8 key = keyboard_get_key();

  if (game_state.game_mode == GAME_MODE_MENU) {
    keyboard_handler_menu(key);
  } else {
    keyboard_handler_game(key);
  }
}

// =========
// Rendering
// =========
void render_menu() {
  for (size_t y = 0; y < SCREEN_HEIGHT; y++) {
    for (size_t x = 10; x < 25; x++) {
      screen_set(x, y, 41);
    }

    for (size_t x = 25; x < 40; x++) {
      screen_set(x, y, 42);
    }

    for (size_t x = 40; x < 55; x++) {
      screen_set(x, y, 43);
    }
  }

  font_char_scaled('P', 100, 30, PRIMARY_COLOUR, 4);
  font_char_scaled('O', 100 + (8 * 4 * 1), 30, PRIMARY_COLOUR, 4);
  font_char_scaled('N', 100 + (8 * 4 * 2), 30, PRIMARY_COLOUR, 4);
  font_char_scaled('G', 100 + (8 * 4 * 3), 30, PRIMARY_COLOUR, 4);

  char *subtitle_str = "(BARELY) WORKING";
  char *player_one_str = "[1] - ONE PLAYER";
  char *player_two_str = "[2] - TWO PLAYER";

  size_t subtitle_len = strlen(subtitle_str);
  size_t player_one_len = strlen(player_one_str);
  size_t player_two_len = strlen(player_two_str);

  font_str(subtitle_str, SCREEN_CENTER_X - ((subtitle_len * 8) / 2), 70, PRIMARY_COLOUR);
  font_str(player_one_str, SCREEN_CENTER_X - ((player_one_len * 8) / 2), 110, PRIMARY_COLOUR);
  font_str(player_two_str, SCREEN_CENTER_X - ((player_two_len * 8) / 2), 130, PRIMARY_COLOUR);
}

void render_game() {
  draw_score();
  draw_game_arena();
  draw_game_arena_border();

  draw_paddle(&left_paddle, left_paddle.colour);
  draw_paddle(&right_paddle, right_paddle.colour);
  draw_ball(&ball);
}

void render() {
  screen_clear(0);

  if (game_state.game_mode == GAME_MODE_MENU) {
    render_menu();
  } else {
    render_game();
  }
    
  screen_swap();
}

int _main() {
    serial_init();
    idt_init();
    isr_init();
    irq_init();
    timer_init();
    keyboard_init();
    fpu_init();

    init();

    game_init();

    u32 last_frame = 0, last = 0;

    while (true) {
      const u64 now = (u64) timer_get();

      if (now != last) {
        last = now;
      }

      keyboard_handler();

      if ((now - last_frame) > 12) {
        last_frame = now;

        if (game_state.game_mode != GAME_MODE_MENU) {
          move_paddle(&right_paddle, ball.yCor > (right_paddle.yCor + 10) ? Down : Up);
          update_ball();
        }

        render();
      }
    }

    return 0;
}

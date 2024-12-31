#include "io.h"
#include "irq.h"
#include "screen.h"
#include "isr.h"
#include "idt.h"
#include "timer.h"
#include "font.h"
#include "keyboard.h"
#include "main.h"

#define FPS 120

#define PADDLE_VEL_Y 20
#define BALL_VEL_X 2
#define BALL_VEL_Y 2

#define PADDLE_PADDING_X 10

#define PRIMARY_COLOUR 15     // White
#define SECONDARY_COLOUR 23   // Grey ish
                     
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

static u8 WINNING_SCORE = 5;

GameState game_state;
Paddle left_paddle, right_paddle;
Ball ball;

// Ball hits wall collision checks
#define BALL_HIT_LEFT_WALL(ball) ((ball).xCor <= 0)
#define BALL_HIT_RIGHT_WALL(ball) ((ball).xCor + (ball).size >= SCREEN_WIDTH)
#define BALL_HIT_TOP_WALL(ball) ((ball).yCor <= 0)
#define BALL_HIT_BOTTOM_WALL(ball) ((ball).yCor + (ball.size) >= SCREEN_HEIGHT)

// Ball hits paddle collision checks
#define BALL_COLLIDES_WITH_LEFT_PADDLE_X(ball, paddle) \
  (((ball).xCor <= ((paddle).xCor) + (paddle).sizeX))
#define BALL_COLLIDES_WITH_LEFT_PADDLE_Y(ball, paddle) \
  (((ball).yCor >= ((paddle).yCor)) && ((ball).yCor <= ((paddle).yCor) + ((paddle).sizeY)))
#define BALL_COLLIDES_WITH_RIGHT_PADDLE_X(ball, paddle) \
  (((ball).xCor <= ((paddle).xCor) + (paddle).sizeX))
#define BALL_COLLIDES_WITH_RIGHT_PADDLE_Y(ball, paddle) \
  (((ball).yCor >= ((paddle).yCor)) && ((ball).yCor <= ((paddle).yCor) + ((paddle).sizeY)))
  
void init() {
  game_state = (GameState) {
    .game_mode = GAME_MODE_MENU,
    .player_one_score = 0,
    .player_two_score = 0,
    .is_counting_down = false,
    .countdown_seconds = 0,
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
  size_t score_offset_left = 8 * 3 * 2;
  size_t score_offset_right = 8 * 3;

  font_char_scaled('0' + game_state.player_one_score, SCREEN_CENTER_X - score_offset_left, 20, 30, 3);
  font_char_scaled('0' + game_state.player_two_score, SCREEN_CENTER_X + score_offset_right, 20, 30, 3);
}

void draw_game_arena() {
  u8 colour = 20;
  u16 yCor = 0;

  u16 top = 0, bottom = SCREEN_HEIGHT;

  while (top < SCREEN_CENTER_Y + 1 && bottom > SCREEN_CENTER_Y - 1) {
    for (u16 x = SCREEN_CENTER_X - 3; x < SCREEN_CENTER_X + 3; x++) {
      screen_set(x, top, colour);
      screen_set(x, bottom, colour);
    }

    if (top % 10 == 0) {
      top += 10;
    } 

    if (top % 10 == 0) {
      bottom -= 10;
    }
    
    top++;
    bottom--;
  }

  // Draw top line
  for (size_t row = 0; row < 4; row++) {
    for (size_t col = 0; col < SCREEN_WIDTH; col++) {
      screen_set(col, row, colour);
    }
  }

  // Draw bottom line
  for (size_t row = SCREEN_HEIGHT - 4; row < SCREEN_HEIGHT; row++) {
    for (size_t col = 0; col < SCREEN_WIDTH; col++) {
      screen_set(col, row, colour);
    }
  }
}

// ==========
// Game logic
// ==========
void countdown() {
  const u8 colours[] = { 41, 42, 43 }; 

  for (size_t i = 3; i > 0; i--) {
    font_char_scaled('0' + i, 100, 100, colours[i], 3);

    screen_swap();

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

void listen_go_back_to_menu(volatile boolean *is_running) {
  while (*is_running) {
    u8 key = keyboard_get_key();

    if (key != 0) {
      write_serial_string("KEY");
      write_serial_char(key);
      write_serial_char('\n');

      if (key == 27) {
        write_serial_string("PRESSED ESCAPE\n");
        menu_init();
        game_state.game_mode = GAME_MODE_MENU;
        *is_running = false;
        break;
      }
    }
  }
}

void update_ball() {
  if (game_state.is_counting_down == true) {
    return;
  }

  ball.xCor += ball.velocityX;
  ball.yCor += ball.velocityY;
 
  if (BALL_HIT_TOP_WALL(ball)) {
    ball.velocityY = -ball.velocityY;
    ball.yCor = 1;
  }

  if (BALL_HIT_BOTTOM_WALL(ball)) {
    ball.velocityY = -ball.velocityY;
    ball.yCor = SCREEN_HEIGHT - ball.size - 1;
  }

  // Calculate offset velocity:
  //  https://gamedev.stackexchange.com/questions/4253/in-pong-how-do-you-calculate-the-balls-direction-when-it-bounces-off-the-paddl 
  if (BALL_HIT_LEFT_WALL(ball)) {
    game_state.player_two_score++;

    if (game_state.player_two_score >= WINNING_SCORE) {
      render_popup("Player 2 has won\n[esc] - Main menu", listen_go_back_to_menu);
      return;
    }

    reset_ball();

    game_state.is_counting_down = true;
    game_state.countdown_seconds = 4;

    left_paddle.yCor = SCREEN_CENTER_Y - (left_paddle.sizeY / 2);
    right_paddle.yCor = SCREEN_CENTER_Y - (right_paddle.sizeY / 2);
   
    ball.velocityX = BALL_VEL_X;
    return;
  }

  if (BALL_HIT_RIGHT_WALL(ball)) {
    game_state.player_one_score++;

    if (game_state.player_one_score >= WINNING_SCORE) {
      render_popup("Player 1 has won\n[esc] - Main menu", listen_go_back_to_menu);
      return;
    }

    reset_ball();

    game_state.is_counting_down = true;
    game_state.countdown_seconds = 4;

    left_paddle.yCor = SCREEN_CENTER_Y - (left_paddle.sizeY / 2);
    right_paddle.yCor = SCREEN_CENTER_Y - (right_paddle.sizeY / 2);

    ball.velocityX = -BALL_VEL_X;
    return;
  }

  u16 lPaddleY = left_paddle.yCor;
  u16 lPaddleX = left_paddle.xCor;

  u16 rPaddleY = right_paddle.yCor;
  u16 rPaddleX = right_paddle.xCor;

  u16 ballX = ball.xCor;
  u16 ballY = ball.yCor;

  if (ballX <= lPaddleX + left_paddle.sizeX &&
      ballY + ball.size >= lPaddleY &&
      ballY <= lPaddleY + left_paddle.sizeY) {
    ball.velocityX = -ball.velocityX;

    // Reposition ball to avoid clipping
    ball.xCor = lPaddleX + left_paddle.sizeX + 1;

    i32 paddle_center = lPaddleY + (left_paddle.sizeY / 2);
    i32 hit_position = ballY + (ball.size / 2) - paddle_center;
    ball.velocityY = (hit_position * 6) / (left_paddle.sizeY / 2);

    if (ball.velocityY == 0) {
      ball.velocityY = (hit_position > 0) ? 1 : -1;
    }
  }

  if (ballX + ball.size >= rPaddleX &&
      ballY + ball.size >= rPaddleY &&
      ballY + rPaddleY + right_paddle.sizeY) {
    ball.velocityX = -ball.velocityX;

    ball.xCor = rPaddleX - ball.size - 1;

    i32 paddle_center = rPaddleY + (right_paddle.sizeY / 2);
    i32 hit_position = ballY + (ball.size / 2) - paddle_center;
    ball.velocityY = (hit_position * 6) / (right_paddle.sizeY / 2);

    if (ball.velocityY == 0) {
      ball.velocityY = (hit_position > 0) ? 1 : -1;
    }
  }

  // Reset ball if it goes off-screen
  if (ballX <= 0 || ballX + ball.size >= SCREEN_WIDTH) {
    ball.xCor = SCREEN_CENTER_X;
    ball.yCor = SCREEN_CENTER_Y;

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
    case 'i':
      if (game_state.game_mode == GAME_MODE_TWO_PLAYERS) {
        move_paddle(&right_paddle, Up);
      }
      break;
    case 'k':
      if (game_state.game_mode == GAME_MODE_TWO_PLAYERS) {
        move_paddle(&right_paddle, Down);
      }
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
void render_popup(const char *label, void (*listener_func)(volatile boolean *running)) {
  u8 banner = 6, xPadding = 20, yPadding = 10, line_height = 8, lines = 0;

  u16 max_label_length = 0;
  u16 label_length = 0;

  for (size_t i = 0; label[i] != '\0'; i++) {
    label_length++;

    if (label[i] == '\n') {
      lines++;

      if (label_length > max_label_length) {
        max_label_length = label_length;
        label_length = 0;
      }
    }
  }

  u16 popup_height = banner + (yPadding * 2) + (lines * line_height);
  u16 popup_width = (max_label_length * 8) + (xPadding * 2);

  size_t startX = SCREEN_CENTER_X - (popup_width / 2);
  size_t startY = SCREEN_CENTER_Y - (popup_height / 2);

  // Draw popup background
  for (size_t y = startY; y < (startY + popup_height + banner); y++) {
    for (size_t x = startX; x < (startX + popup_width); x++) {
      screen_set(x, y, PRIMARY_COLOUR);
    }
  }

  // Draw banner
  for (size_t y = startY; y < startY + banner; y++) {
    u8 colour = 41;
    size_t third_width = popup_width / 3;

    for (size_t x = startX; x < startX + popup_width; x++) {
      if (x >= startX + third_width * 1 && x < startX + third_width * 2) {
        colour = 42;
      } else if (x >= startX + third_width * 2) {
        colour = 43;
      }

      screen_set(x, y, colour);
    }

    colour = 41;
  }

  size_t currentX = startX + xPadding;
  size_t currentY = startY + yPadding + banner;

  // Draw popup text
  for (size_t i = 0; label[i] != '\0'; i++) {
    if (label[i] == '\n') {
      currentX = startX + xPadding;
      currentY += line_height + 2;
    } else {
      font_char(label[i], currentX, currentY, 8);
      currentX += 8;
    }
  }
  
  screen_swap();

  volatile boolean is_running = true;
  while (is_running) {
    if (listener_func != NULL) {
      listener_func(&is_running);
    }
  }
}

void render_menu() {
  static u32 elapsed_time = 0;
  static size_t color_index = 0;

  const u8 primary_colors[] = { 41, 42, 43 };
  const u8 border_colors[] = { 201, 202, 203 };

  const size_t num_colors = sizeof(primary_colors) / sizeof(primary_colors[0]);

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

  // TITLE: "PONG"
  char *title_str = "PONG";
  u8 title_len = strlen(title_str);

  u8 primary_color = primary_colors[color_index];
  u8 border_color = border_colors[color_index];

  for (u8 i = 0; i < title_len; i++) {
    u8 offset = 3;

    font_char_scaled(title_str[i], 100 + (8 * 4 * i) + offset, 30 + offset, border_color, 4);
    font_char_scaled(title_str[i], 100 + (8 * 4 * i), 30, primary_color, 4);
  }

  char *subtitle_str = "(BARELY) WORKING";
  char *player_one_str = "[1] - ONE PLAYER (W/S)";
  char *player_two_str = "[2] - TWO PLAYER (I/K)";

  size_t subtitle_len = strlen(subtitle_str);
  size_t player_one_len = strlen(player_one_str);
  size_t player_two_len = strlen(player_two_str);

  font_str_doubled(subtitle_str, SCREEN_CENTER_X - ((subtitle_len * 8) / 2), 70, 15, 42);
  font_str(player_one_str, SCREEN_CENTER_X - ((player_one_len * 8) / 2), 110, PRIMARY_COLOUR);
  font_str(player_two_str, SCREEN_CENTER_X - ((player_two_len * 8) / 2), 130, PRIMARY_COLOUR);

  if (timer_get() - elapsed_time >= 500) {
    elapsed_time = timer_get();
    color_index = (color_index + 1) % num_colors;
  }
}

void render_game() {
  static u32 countdown_elapsed_time = 0;

  draw_score();
  draw_game_arena();

  draw_paddle(&left_paddle, left_paddle.colour);
  draw_paddle(&right_paddle, right_paddle.colour);
  draw_ball(&ball);

  if (game_state.is_counting_down == true) {
    font_char_scaled('0' + game_state.countdown_seconds, 100, 100, 41, 3);
    
    if (timer_get() - countdown_elapsed_time >= 1000) {
      countdown_elapsed_time = timer_get();
      game_state.countdown_seconds--;

      if (game_state.countdown_seconds == 0) {
        game_state.is_counting_down = false;
      }
    }
  }
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

    seed(42);

    init();

    game_init();

    u32 last_frame = 0, last = 0;

    while (true) {
      const u64 now = (u64) timer_get();

      if (now != last) {
        last = now;
      }

      if (game_state.is_counting_down == false) {
        keyboard_handler();
      }

      if ((now - last_frame) > 12) {
        last_frame = now;
 
        if (game_state.game_mode == GAME_MODE_ONE_PLAYER && game_state.is_counting_down == false) {
          u32 randomness = rand() % 100; // Random number between 0 and 100
                                              
          // Predict the ball Y position when it reaches the paddle
          i32 predicted_y = ball.yCor + (ball.velocityY * abs(right_paddle.xCor - ball.xCor) / abs(ball.velocityX));

          // Misprediction logic
          if (randomness < 30) { // 30% chance
            predicted_y += (rand() % 21) - 10; // Offset by -10 to +10 pixels
          } else if (randomness < 50) { // Additional 20%
            predicted_y += (rand() % 11) - 5; // Offset by -5 to +5 pixels
          }

          // Buffer to reduce jitter
          int paddle_center = right_paddle.yCor + (right_paddle.sizeY / 2);
          int movement_treshold = 12;
          int buffer = 5;

          if (predicted_y > paddle_center + buffer + movement_treshold) {
            move_paddle(&right_paddle, Down);
          } else if (predicted_y < paddle_center - buffer - movement_treshold) {
            move_paddle(&right_paddle, Up);
          }
        }

        if (game_state.game_mode != GAME_MODE_MENU) {
          update_ball();
        }

        render();
      }
    }

    return 0;
}

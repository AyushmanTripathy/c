#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

#define HEIGHT 720
#define WIDTH 1200
#define FRAME_DELAY 30
#define BAR_WIDTH 30 
#define BAR_HEIGHT 200
#define BAR_BOUNCE_FACTOR 2
#define BALL_SIZE 40
#define BORDER_WIDTH 10
#define MOMENTUM_LENGTH 5

typedef struct {
  int x;
  int y;
  int dx;
  int dy;
  int h;
  int w;
  int c;
} entity;

SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;
char running = 1, paused = 0;

// NOTE: border is 1 pixel in width and height
void move_bar(entity * bar) {
  if (bar->y <= BORDER_WIDTH) bar->y = BORDER_WIDTH;
  if (bar->y + bar->h >= HEIGHT - BORDER_WIDTH) bar->y = HEIGHT - bar->h - BORDER_WIDTH;
}

char is_vertical_collision(entity * a, entity * b) {
  if (a->y <= b->y + b->h && a->y + a->h >= b->y) return 1;
  return 0;
}

void move_ball(entity * ball, entity * left, entity * right) {
  // reflect from top and bottom
  if (ball->y <= BORDER_WIDTH || ball->y + ball->h >= HEIGHT - BORDER_WIDTH) ball->dy *= -1;
  // death
  if (ball->x <= BORDER_WIDTH || ball->x + ball->w >= WIDTH - BORDER_WIDTH) {
    ball->c = 2;
    running = 0;
    return;
  }

  if (left->x + left->w == ball->x && is_vertical_collision(left, ball)) {
    ball->dy = left->dy;
    printf("%d\n", ball->dy);
    ball->dx *= -1;
  }
  if (right->x == ball->x + ball->w && is_vertical_collision(right, ball)) {
    ball->dy = right->dy;
    printf("%d\n", ball->dy);
    ball->dx *= -1;
  }

  ball->x += ball->dx;
  ball->y += ball->dy;
  if (ball->y <= BORDER_WIDTH) ball->y = BORDER_WIDTH;
  if (ball->y + ball->h >= HEIGHT - BORDER_WIDTH) ball->y = HEIGHT - BORDER_WIDTH - ball->h;
}

void draw_entity(entity * e) {
  static SDL_Rect r;
  r.x = e->x;
  r.y = e->y;
  r.h = e->h;
  r.w = e->w;
  if (e->c == 2) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  else if (e->c == 3) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

  SDL_RenderFillRect(renderer, &r);
}

void draw_boundries() {
  // borders
  SDL_Rect border = {0, 0, BORDER_WIDTH, WIDTH};
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &border);
  border.x = WIDTH - BORDER_WIDTH;
  SDL_RenderFillRect(renderer, &border);
  border.x = WIDTH - BORDER_WIDTH;
  border.x = 0;
  border.h = BORDER_WIDTH;
  border.w = WIDTH;
  SDL_RenderFillRect(renderer, &border);
  border.y = HEIGHT - BORDER_WIDTH;
  SDL_RenderFillRect(renderer, &border);
}

int calc_bar_speed(int * speeds, int * speed_index, int delta_speed) {
  speeds[*speed_index] = delta_speed / BAR_BOUNCE_FACTOR;
  int total = 0;
  for (int i = 0; i < MOMENTUM_LENGTH; i++)
    total += speeds[i];
  *speed_index = (*speed_index + 1) % MOMENTUM_LENGTH;
  return total;
}

int game_loop() {
  SDL_Event e;
  char first_mouse_state = 1;

  entity left = { 100, HEIGHT / 2 - BAR_HEIGHT / 2, 0, 0, BAR_HEIGHT, BAR_WIDTH, 2 };
  entity right = { WIDTH - BAR_WIDTH - 100, HEIGHT / 2 - BAR_HEIGHT / 2, 0, 0, BAR_HEIGHT, BAR_WIDTH, 2 };
  entity ball = { WIDTH / 2 - BALL_SIZE / 2, HEIGHT / 2 - BALL_SIZE / 2, 10, 10, BALL_SIZE, BALL_SIZE, 3 };

  int left_momentum[MOMENTUM_LENGTH] = {0}, left_momentum_index = 0;
  int right_momentum[MOMENTUM_LENGTH] = {0}, right_momentum_index = 0;
  for (int i = 0; i < MOMENTUM_LENGTH; i++) {
    left_momentum[i] = 0;
    right_momentum[i] = 0;
  }

  while(running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) return 1;
      else if (e.type == SDL_MOUSEMOTION) {
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);

        if (first_mouse_state) {
          left.dy = calc_bar_speed(left_momentum, &left_momentum_index, 0);
          right.dy = calc_bar_speed(right_momentum, &right_momentum_index, 0);
          first_mouse_state = 0;
        } else {
          left.dy = calc_bar_speed(left_momentum, &left_momentum_index, mouse_y - left.y);
          right.dy = calc_bar_speed(right_momentum, &right_momentum_index, mouse_y - right.y);
        }

        left.y = mouse_y;
        right.y = mouse_y;
      } else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_q:
            return 1;
          case SDLK_SPACE:
            paused = paused ? 0 : 1;
            break;
        }
      }
    }
    SDL_Delay(FRAME_DELAY);
    if (paused) continue;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_boundries();

    move_ball(&ball, &left, &right);
    move_bar(&left);
    move_bar(&right);

    //draw bars
    draw_entity(&left);
    draw_entity(&right);
    draw_entity(&ball);

    SDL_RenderPresent(renderer);
    left_momentum[left_momentum_index] = 0;
    left_momentum_index = (left_momentum_index + 1) % MOMENTUM_LENGTH;
    right_momentum[right_momentum_index] = 0;
    right_momentum_index = (right_momentum_index + 1) % MOMENTUM_LENGTH;
  }
  return 0;
}
int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

  while (1) {
    running = 1;
    paused = 0;
    if (running) { 
      if (game_loop()) return 0;
      SDL_Delay(1000);
    }
  }
  return 0;
}

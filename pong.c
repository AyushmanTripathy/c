#include <SDL2/SDL_render.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>


#define HEIGHT 72
#define WIDTH 120
#define FRAME_DELAY 30
#define PIXEL_SIZE 10
#define BAR_WIDTH 1 
#define BAR_HEIGHT 20
#define BAR_SPEED 3 
#define BALL_SIZE 4

typedef struct {
  int x;
  int y;
  int dx;
  int dy;
  int h;
  int w;
} entity;


SDL_Window * window = NULL;
SDL_Renderer * renderer = NULL;
char running = 1, paused = 0;

void move_entity(entity * e) {
  e->x += e->dx;
  e->y += e->dy;
  if (e->x < 0) e->x = 0;
  if (e->y < 0) e->y = 0;
  if (e->x > WIDTH) e->x = WIDTH;
  if (e->y > HEIGHT) e->y = HEIGHT;
}

// NOTE: border is 1 pixel in width and height
void move_bar(entity * bar) {
  move_entity(bar);
  if (bar->y <= 1) bar->y = 1;
  if (bar->y + bar->h >= HEIGHT - 1) bar->y = HEIGHT - bar->h - 1;
  bar->dy = 0;
}

char is_vertical_collision(entity * a, entity * b) {
  if (a->y <= b->y + b->h && a->y + a->h >= b->y) return 1;
  return 0;
}

void move_ball(entity * ball, entity * left, entity * right) {
  // reflect from top and bottom
  if (ball->y == 1 || ball->y + ball->h == HEIGHT - 1) ball->dy *= -1;
  // death
  if (ball->x == 1 || ball->x + ball->w == WIDTH - 1) {
    paused = 1;
    return;
  }
  
  if (left->x + left->w == ball->x && is_vertical_collision(left, ball)) ball->dx *= -1;
  if (right->x == ball->x + ball->w && is_vertical_collision(right, ball)) ball->dx *= -1;
  move_entity(ball);
}

void draw_entity(entity * e) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (int x = 0; x < e->w; x++) {
    for (int y = 0; y < e->h; y++)
      SDL_RenderDrawPoint(renderer, e->x + x, e->y + y);
  }
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(WIDTH * PIXEL_SIZE, HEIGHT * PIXEL_SIZE, 0, &window, &renderer);
  SDL_RenderSetScale(renderer, PIXEL_SIZE, PIXEL_SIZE);

  SDL_Event e;

  entity bar1 = { 5, HEIGHT / 2 - BAR_HEIGHT / 2, 0, 0, BAR_HEIGHT, BAR_WIDTH};
  entity bar2 = { WIDTH - BAR_WIDTH - 5, HEIGHT / 2 - BAR_HEIGHT / 2, 0, 0, BAR_HEIGHT, BAR_WIDTH};
  entity ball = { WIDTH / 2 - BALL_SIZE / 2, HEIGHT / 2 - BALL_SIZE / 2, 1, 1, BALL_SIZE, BALL_SIZE};

  while(running) {
    SDL_Delay(FRAME_DELAY);
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = 0;
      else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_q:
            running = 0;
            break;
          case SDLK_DOWN:
            bar1.dy = BAR_SPEED;
            bar2.dy = BAR_SPEED;
            break;
          case SDLK_UP:
            bar1.dy = -BAR_SPEED;
            bar2.dy = -BAR_SPEED;
            break;
          case SDLK_SPACE:
            paused = paused ? 0 : 1;
            break;
        }
      }
    }
    if (paused) continue;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // borders
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int y = 0; y < HEIGHT; y++) {
        SDL_RenderDrawPoint(renderer, 0, y);
        SDL_RenderDrawPoint(renderer, WIDTH - 1, y);
    }
    for (int x = 0; x < WIDTH; x++) {
        SDL_RenderDrawPoint(renderer, x, 0);
        SDL_RenderDrawPoint(renderer, x, HEIGHT - 1);
    }

    move_ball(&ball, &bar1, &bar2);
    move_bar(&bar1);
    move_bar(&bar2);


    //draw bars
    draw_entity(&bar1);
    draw_entity(&bar2);
    draw_entity(&ball);

    SDL_RenderPresent(renderer);
  }
  return 0;
}

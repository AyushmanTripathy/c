#include <SDL2/SDL.h>
#include <random>
#include <iostream>

const int grid_size = 50;
const int pixel_size = 20;

int perms[8][2] = {{ 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 }, { 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }};

int main() {
  SDL_Window * window = nullptr;
  SDL_Renderer * renderer = nullptr;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(grid_size * pixel_size, grid_size * pixel_size, 0, &window, &renderer);
  SDL_RenderSetScale(renderer, pixel_size, pixel_size);

  std::random_device dev;
  std::uniform_int_distribution<int> nd(0,50);

  SDL_Event e;
  bool running = true, paused = false;
  int map[grid_size + 2][grid_size + 2], prev[grid_size + 2][grid_size + 2];

  for (int i = 0; i < grid_size + 2; i++) {
    for (int j = 0; j < grid_size + 2; j++) {
      int tmp = nd(dev) > 45 ? 1 : 0;
      map[i][j] = tmp;
      prev[i][j] = tmp;
    }
  }

  while (running) {
    while(SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = false;
      else if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        x = x % pixel_size, y = y % pixel_size;
        if (x < grid_size + 2 && y < grid_size + 2)
          prev[x][y] = 1;
      } else if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.sym) {
          case SDLK_SPACE:
            paused = paused ? false : true;
            break;
          case SDLK_q:
            running = false;
            break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 1; i < grid_size; i++) {
      for (int j = 1; j < grid_size; j++) {
        if (prev[i][j]) SDL_RenderDrawPoint(renderer, i, j);
      }
    }

    if (paused) continue;

    for (int i = 1; i < grid_size; i++) {
      for (int j = 1; j < grid_size; j++) {
        int count = 0;
        for (int x = 0; x < 8; x++) {
          count += prev[i + perms[x][0]][j + perms[x][1]];
        }
        if (count < 2 || count > 3) map[i][j] = 0;
        else map[i][j] = 1;
      }
    }

    for (int i = 0; i < grid_size + 2; i++) {
      for (int j = 0; j < grid_size + 2; j++)
        prev[i][j] = map[i][j];
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(1000);
  }
  return 0;
}

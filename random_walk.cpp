#include <SDL2/SDL.h>
#include <random>
#include <iostream>

int main() {
  const int grid_size = 50;
  const int pixel_size = 20;

  SDL_Window * window = nullptr;
  SDL_Renderer * renderer = nullptr;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer(grid_size * pixel_size, grid_size * pixel_size, 0, &window, &renderer);
  SDL_RenderSetScale(renderer, pixel_size, pixel_size);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  std::random_device dev;
  std::uniform_int_distribution<uint8_t> nd(0,4);

  SDL_Event e;
  bool running = true, paused = false;
  int x = grid_size / 2, y = grid_size / 2;

  while (running) {
    while(SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = false;
      else if (e.type == SDL_KEYUP) {
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

    if (paused) continue;

    uint8_t dir = nd(dev);
    switch(dir) {
      case 0:
        x += 1;
        break;
      case 1:
        x += -1;
        break;
      case 2:
        y += 1;
        break;
      case 3:
        y += -1;
        break;
    }

    if (x > grid_size || x < 0 || y > grid_size || y < 0) x = grid_size / 2, y = grid_size / 2;

    int color = 20 * dir;
    SDL_SetRenderDrawColor(renderer, color, color, color, color);
    SDL_RenderDrawPoint(renderer, x, y);
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
  }
  return 0;
}

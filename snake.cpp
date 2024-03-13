#include <SDL2/SDL.h>
#include <random>
#include <iostream>
#include <limits.h>
#include <stack>

const int grid_size = 50;
const int pixel_size = 20;

std::random_device dev;
std::uniform_int_distribution<int> nd(0,grid_size-1);

int perms[4][2] = {{ 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 }};

float distance(int * src, int x, int y) {
  return sqrt(pow(src[0] - x, 2) + pow(src[1] - y, 2));
}

bool check_for_collision(int body[][2], int body_length, int * x) {
  for (int i = 0; i < body_length - 1; i++) {
    if (body[i][0] == x[0] && body[i][1] == x[1]) {
      return true;
    }
  }
  return false;
}

void bound_to_range(int * x) {
  if (*x >= grid_size) *x = 0;
  else if (*x < 0) *x = grid_size - 1;
}

int main() {
  SDL_Window * window = nullptr;
  SDL_Renderer * renderer = nullptr;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer((grid_size + 2) * pixel_size, (grid_size + 2) * pixel_size, 0, &window, &renderer);
  SDL_RenderSetScale(renderer, pixel_size, pixel_size);

  SDL_Event e;
  bool running = true, paused = false;

  int length = 3, dir = 1, prev_dir = 1;
  int curr[grid_size * grid_size][2], food[2];
  int * head;

  food[0] = 8;
  food[1] = 2;

  curr[0][0] = 2;
  curr[1][0] = 2;
  curr[2][0] = 2;

  curr[0][1] = 2;
  curr[1][1] = 2;
  curr[2][1] = 2;

  head = curr[2];

  while (running) {
    while(SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = false;
      else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
          case SDLK_SPACE:
            paused = paused ? false : true;
            break;
          case SDLK_q:
            running = false;
            break;
          case SDLK_UP:
            dir = 2;
            break;
          case SDLK_DOWN:
            dir = 0;
            break;
          case SDLK_RIGHT:
            dir = 1;
            break;
          case SDLK_LEFT:
            dir = 3;
            break;
        }
      }
    }

    if (paused) continue;

    if (length == grid_size * grid_size - 1) paused = true;

    if (prev_dir != dir) {
      if (curr[length - 2][0] == head[0] + perms[dir][0] && 
          curr[length - 2][1] == head[1] + perms[dir][1])
        dir = prev_dir;
      else prev_dir = dir;
    }

    if (food[0] == head[0] && food[1] == head[1]) {
      curr[length][0] = food[0];
      curr[length][1] = food[1];
      head = curr[length];
      length++;

      // selecting new food
      bool found = false;
      while(!found) {
        food[0] = nd(dev);
        food[1] = nd(dev);
        found = true;
        for (int i = 0; i < length; i++) {
          if (food[0] == curr[i][0] && food[1] == curr[i][1])
            found = false;
        }
      }
    } else {
      // moving the snake
      for (int i = 0; i < length - 1; i++) {
        curr[i][0] = curr[i + 1][0];
        curr[i][1] = curr[i + 1][1];
      }
    } 
    head[0] += perms[dir][0];
    head[1] += perms[dir][1];

    bound_to_range(&head[0]);
    bound_to_range(&head[1]);

    // painting
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < grid_size + 2; i++) {
      SDL_RenderDrawPoint(renderer, 0, i);
      SDL_RenderDrawPoint(renderer, i, 0);
      SDL_RenderDrawPoint(renderer, grid_size + 1, i);
      SDL_RenderDrawPoint(renderer, i, grid_size + 1);
    }
    for (int i = 0; i < length; i++) {
     SDL_RenderDrawPoint(renderer, curr[i][0]+1, curr[i][1]+1);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawPoint(renderer, food[0] + 1, food[1] + 1);

    SDL_RenderPresent(renderer);
    SDL_Delay(10);

    if (check_for_collision(curr, length, head)) {
      running = false;
      SDL_Delay(1000);
    }

    // check for best dir
    float min = FLT_MAX;
    for(int i = 0; i < 4; i++) {
      head[0] += perms[i][0];
      head[1] += perms[i][1];
      if (!check_for_collision(curr, length, head)) {
        float tmp_dist = distance(food, head[0]+perms[i][0], head[1]+perms[i][1]);
        if (tmp_dist < min) {
          min = tmp_dist;
          dir = i;
        }
      }
      head[0]-=perms[i][0];
      head[1]-=perms[i][1];
    } 
    if (min == FLT_MAX)  {
      std::cout << "giving up... \n";
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderDrawPoint(renderer, head[0]+1, head[1]+1);
      SDL_Delay(3000);
      running = false;
    }
  }
  std::cout << "score: " << length << "\n";
  return 0;
}

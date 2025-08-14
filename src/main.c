// #include <SDL/SDL.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_time.h>
#include <SDL3/SDL_video.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 Arena allocations here
 * */

// typedef struct Arena {
//   size_t size;
//   size_t capacity;
//   size_t position;
//   void *data;
// } Arena;
//
// Arena *ArenaAlloc(size_t capacity) {
//   Arena *arena = malloc(sizeof(Arena));
//   arena->size = 0;
//   arena->capacity = capacity;
//   arena->position = 0;
//   arena->data = malloc(sizeof(size_t) * capacity);
//   return arena;
// }
//
// void ArenaRelease(Arena *arena) {
//   free(arena->data);
//   free(arena);
// }

// Global Vars
bool is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;
int window_width = 0;
int window_height = 0;
float window_refresh = 0.0f;

// initialize frame/color buffer to null pointer
uint32_t *color_buffer = NULL;

// int setenv("SDL_VIDEODRIVER", "x11", 1);

bool initialize_window(void) {
  int init_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  if (init_result < 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  // SDL query for display max width and height. SDL2 Old implementation
  // SDL_DisplayMode display_mode;
  // SDL_GetCurrentDisplayMode(0, &display_mode);
  // window_width = display_mode.w;
  // window_height = display_mode.h;

  // SDL query for display max width and height. SDL3
  SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
  const SDL_DisplayMode *display_mode = SDL_GetCurrentDisplayMode(display_id);
  SDL_GetCurrentDisplayMode(0);
  window_width = display_mode->w;
  window_height = display_mode->h;
  window_refresh = display_mode->refresh_rate;
  fprintf(stdout, "display_mode.w = %d.\n", window_width);
  fprintf(stdout, "display_mode.h = %d.\n", window_height);
  fprintf(stdout, "display_mode.refresh_rate = %0.f.\n", window_refresh);

  // Create a SDL Window
  window = SDL_CreateWindow(NULL, window_width, window_height,
                            SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  // Create a SDL renderer
  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }
  /*
    ISSUE:
    if the window is moved, then the quit escape key cannot be used for some
    reason..even happens with the borderless window setup where the escape is
    not registered until after multiple tries.
    */
  // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  return true;
}

void setup(void) {
  // allocated the memory for the frame/color buffer
  // casing to uint32_t pointer
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  // Trying Arena instead of malloc
  //  color_buffer =
  //      (uint32_t *)ArenaAlloc(sizeof(uint32_t) * window_width *
  //      window_height);
  if (!color_buffer) {
    fprintf(stderr, "Error allocating color_buffer memory in Arena.\n");
  }

  // Creating SDL texture for displaying colors in the color_buffer /
  // frame_buffer.
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);

  // if the malloc function fails, it will return a NULL pointer which is what
  // we are checking above. everytime we use malloc we need to free the
  // allocated memory.
}
void process_input(void) {
  // FIXED by changing from switch case to if statements.
  SDL_Event event;
  SDL_PollEvent(&event);

  if (event.type == SDL_EVENT_KEY_DOWN) {
    if (event.key.key == SDLK_ESCAPE) {
      is_running = false;
    }
  }

  // switch (event.type) {
  // case SDL_EVENT_QUIT:
  //   is_running = false;
  //   break;
  // case SDL_EVENT_KEY_DOWN:
  //   if (event.key.key == SDLK_ESCAPE) {
  //     is_running = false;
  //   }
  //   break;
  // }
}
void update(void) {
  // TODO:
}

// draw_grid exercise test..
void draw_grid(void) {
  for (int y = 0; y < window_height;
       y += 50) { // Draw horizontal lines at every 50 pixels
    SDL_RenderLine(renderer, 0, y, window_width, y);
  }
  for (int x = 0; x < window_width;
       x += 50) { // Draw vertical lines at every 50 pixels
    SDL_RenderLine(renderer, x, 0, x, window_height);
  }
}

void render_color_buffer(void) {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                    (int)(window_width * sizeof(uint32_t)));
  // SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
  SDL_RenderTexture(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      color_buffer[(window_width * y) + x] = color;
    }
  }
}
void render(void) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderClear(renderer);
  render_color_buffer();
  clear_color_buffer(0xFF000000);
  draw_grid();
  // draw_color();
  SDL_RenderPresent(renderer);
}

// Basically free the memory
// our own GC
void destroy_window(void) {
  free(color_buffer);
  // ArenaRelease(color_buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
int main(void) {
  setenv("SDL_VIDEODRIVER", "x11", 1);
  printf("SDL_VIDEODRIVER=%s\n", getenv("SDL_VIDEODRIVER"));
  is_running = initialize_window();

  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }
  destroy_window();

  return 0;
}

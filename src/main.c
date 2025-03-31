//#include <SDL/SDL.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Global Vars
bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int window_width = 800;
int window_height = 600;

//int setenv("SDL_VIDEODRIVER", "x11", 1);

bool initialize_window(void) {
    int init_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (init_result < 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    // Create a SDL Window
    window = SDL_CreateWindow(
        NULL,
        window_width,
        window_height,
        SDL_WINDOW_BORDERLESS
    );
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

    return true;
}


void setup(void) {
   // TODO:

}
void process_input(void) {
    // TODO:
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type) {
        case SDL_EVENT_QUIT:
            is_running = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (event.key.key == SDLK_ESCAPE)
                is_running = false;
            break;
    }
}
void update(void) {
    // TODO:

}
void render(void) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int main(void) {
    setenv("SDL_VIDEODRIVER", "x11", 1);
    printf("SDL_VIDEODRIVER=%s\n", getenv("SDL_VIDEODRIVER"));
    is_running = initialize_window();

    setup();

    while(is_running) {
        process_input();
        update();
        render();
    }

    return 0;
}

#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdlib.h>

#define WIDTH 640
#define HEIGHT 480

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
	SDL_SetAppMetadata("A 3D Game", "0.1", "com.wowmuchdoge.funnygame");

	AppState* appState = (AppState*)malloc(sizeof(AppState));

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Error: Could not initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("3D Game", WIDTH, HEIGHT, 0, &appState->window, &appState->renderer)) {
		SDL_Log("Error: Could not create window/renderer %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appState, SDL_Event* event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appState) {
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appState, SDL_AppResult result) {

}

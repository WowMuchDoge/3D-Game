#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "player.h"

#define WIDTH 640
#define HEIGHT 480

#define PIXELS_PER_METER 100.0

// sqrt(0.5^2 + 0.5^2)
#define PLAYER_HYPOT 0.70710678118

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
	Player player;
} AppState;

typedef struct {
	int x;
	int y;
} Coord;

double DegreesToRadians(double deg) {
	return (deg / 180) * PI;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
	SDL_SetAppMetadata("A 3D Game", "0.1", "com.wowmuchdoge.funnygame");

	AppState* appState = (AppState*)malloc(sizeof(AppState));

	InitPlayer(&appState->player);

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Error: Could not initialize SDL: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("3D Game", WIDTH, HEIGHT, 0, &appState->window, &appState->renderer)) {
		SDL_Log("Error: Could not create window/renderer %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	*appstate = appState;

	return SDL_APP_CONTINUE;
}

void DrawPolygon(AppState* appState, Coord* coords, int size) {
	SDL_SetRenderDrawColor(appState->renderer, 0, 255, 0, 255);
	for (int i = 0; i < size - 1; i++) {
		SDL_RenderLine(appState->renderer, coords[i].x, coords[i].y, coords[i + 1].x, coords[i + 1].y);
	}

	SDL_RenderLine(appState->renderer, coords[size - 1].x, coords[size - 1].y, coords[0].x, coords[0].y);
	SDL_SetRenderDrawColor(appState->renderer, 0, 0, 0, 255);
}

SDL_AppResult SDL_AppEvent(void *appState, SDL_Event* event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}
	
	return SDL_APP_CONTINUE;
}

Coord PositionToPixel(double x, double y) {
	return (Coord){.x = (int)(x * PIXELS_PER_METER), .y = (int)(y * PIXELS_PER_METER)};
}

SDL_AppResult SDL_AppIterate(void *appState) {

	AppState* as = (AppState*)appState;

	const bool *keystate;

	keystate = SDL_GetKeyboardState(NULL);
	double xspeed = 0, yspeed = 0, rotspeed = 0;

	if (keystate[SDL_SCANCODE_W]) yspeed -= 0.5;
	if (keystate[SDL_SCANCODE_S]) yspeed += 0.5;
	if (keystate[SDL_SCANCODE_A]) xspeed -= 0.5;
	if (keystate[SDL_SCANCODE_D]) xspeed += 0.5;
	if (keystate[SDL_SCANCODE_1]) rotspeed += 90.0;
	if (keystate[SDL_SCANCODE_2]) rotspeed -= 90.0;

	MovePlayer(&as->player, (Speeds){.vx = xspeed, .vy = yspeed, .omega = rotspeed});

	IteratePlayer(&as->player);

	Coord playerCoords[] = {
		PositionToPixel(as->player.x + PLAYER_HYPOT * cos(DegreesToRadians(as->player.rot - 45)), as->player.y + PLAYER_HYPOT * sin(DegreesToRadians(as->player.rot - 45))),
		PositionToPixel(as->player.x + PLAYER_HYPOT * cos(DegreesToRadians(as->player.rot + 45)), as->player.y + PLAYER_HYPOT * sin(DegreesToRadians(as->player.rot + 45))),
		PositionToPixel(as->player.x + PLAYER_HYPOT * cos(DegreesToRadians(as->player.rot + 135)), as->player.y + PLAYER_HYPOT * sin(DegreesToRadians(as->player.rot + 135))),
		PositionToPixel(as->player.x + PLAYER_HYPOT * cos(DegreesToRadians(as->player.rot + 225)), as->player.y + PLAYER_HYPOT * sin(DegreesToRadians(as->player.rot + 225)))
	};

	// printf("%d, %d\n", PositionToPixel(as->player.x, as->player.y).x, PositionToPixel(as->player.x, as->player.y).y);

	SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
	DrawPolygon(as, playerCoords, 4);
	SDL_RenderPresent(as->renderer);
	SDL_RenderClear(as->renderer);

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appState, SDL_AppResult result) {

}

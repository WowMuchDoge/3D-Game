#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "player.h"
#include "polygon.h"

#define WIDTH 640
#define HEIGHT 480

#define PIXELS_PER_METER 100.0
#define FOV 90

// sqrt(0.5^2 + 0.5^2)
#define PLAYER_HYPOT 0.70710678118

typedef struct {
	SDL_Renderer* renderer;
	SDL_Window* window;
	Player player;
	World world;
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

	appState->player = InitPlayer(FOV, WIDTH, HEIGHT);

	Point3d point = (Point3d){1.0, 3.0, 2.0};
	Point3d playerPoint = ToPlayerCoordinatePoint3d(point, appState->player);
	Point2d projectedPoint = ProjectPointTo2d(playerPoint, appState->player);

	// printf("World X: %f, World Y: %f, World Z: %f\n", point.x, point.y, point.z); 
	// printf("Player X: %f, Player Y: %f, Player Z: %f\n", playerPoint.x, playerPoint.y, playerPoint.z); 
	// printf("Projected X: %f, Projected Y: %f\n", projectedPoint.x, projectedPoint.y);

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
	if (keystate[SDL_SCANCODE_A]) rotspeed -= 90.0;
	if (keystate[SDL_SCANCODE_D]) rotspeed += 90.0;

	MovePlayer(&as->player, (Speeds){.vx = 0, .vy = yspeed, .omega = rotspeed});

	IteratePlayer(&as->player);

	// Polygon3d playerPoly = ToPlayerCoordinatePolygon3d(as->world.worldPolygons[0], as->player);
	// Polygon2d projectedPoly = ProjectPolygonTo2d(playerPoly, as->player);
	//
	// Point2d p1 = projectedPoly.vertices[0];
	// Point2d p2 = projectedPoly.vertices[1];
	Point3d p1 = (Point3d){1.0, 3.0, 2.0};
	Point3d playerPoint1 = ToPlayerCoordinatePoint3d(p1, as->player);
	Point2d projectedPoint1 = ProjectPointTo2d(playerPoint1, as->player);

	Point3d p2 = (Point3d){2.0, 3.0, 2.0};
	Point3d playerPoint2 = ToPlayerCoordinatePoint3d(p2, as->player);
	Point2d projectedPoint2 = ProjectPointTo2d(playerPoint2, as->player);

	Point3d p3 = (Point3d){2.0, 3.0, 1.0};
	Point3d playerPoint3 = ToPlayerCoordinatePoint3d(p3, as->player);
	Point2d projectedPoint3 = ProjectPointTo2d(playerPoint3, as->player);

	Point3d p4 = (Point3d){1.0, 3.0, 1.0};
	Point3d playerPoint4 = ToPlayerCoordinatePoint3d(p4, as->player);
	Point2d projectedPoint4 = ProjectPointTo2d(playerPoint4, as->player);

	SDL_FRect rect1 = {(int)((WIDTH / 2) + projectedPoint1.x * PIXELS_PER_METER), (int)((HEIGHT / 2) + projectedPoint1.y * PIXELS_PER_METER), 50 / playerPoint1.y, 50 / playerPoint1.y};
	SDL_FRect rect2 = {(int)((WIDTH / 2) + projectedPoint2.x * PIXELS_PER_METER), (int)((HEIGHT / 2) + projectedPoint2.y * PIXELS_PER_METER), 50 / playerPoint2.y, 50 / playerPoint2.y};
	SDL_FRect rect3 = {(int)((WIDTH / 2) + projectedPoint3.x * PIXELS_PER_METER), (int)((HEIGHT / 2) + projectedPoint3.y * PIXELS_PER_METER), 50 / playerPoint3.y, 50 / playerPoint3.y};
	SDL_FRect rect4 = {(int)((WIDTH / 2) + projectedPoint4.x * PIXELS_PER_METER), (int)((HEIGHT / 2) + projectedPoint4.y * PIXELS_PER_METER), 50 / playerPoint4.y, 50 / playerPoint4.y};

	SDL_SetRenderDrawColor(as->renderer, 30, 70, 127, 255);	
	SDL_RenderLine(as->renderer, rect1.x, rect1.y, rect2.x, rect2.y);
	SDL_RenderLine(as->renderer, rect2.x, rect2.y, rect3.x, rect3.y);
	SDL_RenderLine(as->renderer, rect3.x, rect3.y, rect4.x, rect4.y);
	SDL_RenderLine(as->renderer, rect4.x, rect4.y, rect1.x, rect1.y);
	SDL_RenderPresent(as->renderer);
	SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
	SDL_RenderClear(as->renderer);

	//
	// SDL_FRect r1 = {(int)(p1.x * PIXELS_PER_METER), (int)(p1.y * PIXELS_PER_METER), 5.0, 5.0};
	// SDL_FRect r2 = {(int)(p2.x * PIXELS_PER_METER), (int)(p2.y * PIXELS_PER_METER), 5.0, 5.0};
	//
	// printf("X1 %f, X2 %f\n", p1.x, p1.y);
	//

	//
	// Coord playerCoords[] = {
	// 	PositionToPixel(as->player.x + PLAYER_HYPOT * cos(DegreesToRadians(as->player.rot - 45)), as->player.y + PLAYER_HYPOT * sin(DegreesToRadians(as->player.rot - 45))),
	// 	PositionToPixel(as->player.x + PLAYER_HYPOT * cos(DegreesToRadians(as->player.rot + 45)), as->player.y + PLAYER_HYPOT * sin(DegreesToRadians(as->player.rot + 45))),
	// 	PositionToPixel(as->player.x + PLAYER_HYPOT * cos(DegreesToRadians(as->player.rot + 135)), as->player.y + PLAYER_HYPOT * sin(DegreesToRadians(as->player.rot + 135))),
	// 	PositionToPixel(as->player.x + PLAYER_HYPOT * cos(DegreesToRadians(as->player.rot + 225)), as->player.y + PLAYER_HYPOT * sin(DegreesToRadians(as->player.rot + 225)))
	// };

	// printf("%d, %d\n", PositionToPixel(as->player.x, as->player.y).x, PositionToPixel(as->player.x, as->player.y).y);

	// SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
	// DrawPolygon(as, playerCoords, 4);
	// SDL_RenderPresent(as->renderer);
	// SDL_RenderClear(as->renderer);

	return SDL_APP_CONTINUE;
}

// SDL handles freeing all the crap for us
void SDL_AppQuit(void *appState, SDL_AppResult result) {
	FreeWorld(((AppState*)appState)->world);
	free(appState);
}

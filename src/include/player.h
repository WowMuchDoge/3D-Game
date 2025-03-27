#ifndef PLAYER_H_
#define PLAYER_H_

#include <sys/time.h>

#define PI 3.14159265359

#define PLAYER_CAMERA_HEIGHT 2
#define PLAYER_METERS_PER_PIXEL 0.01

// Units arbitrarily in m/s for linear coords and degrees/s
// for rotational
typedef struct {
	double vx;
	double vy;
	double omega;
} Speeds;

typedef struct {
	double x;
	double y;
	double rot;
	Speeds _curSpeed;
	struct timespec _curTime;
	int screenWidth;
	int screenHeight;
	double FOV;
} Player;

// Initialized the player with relevant data
Player InitPlayer(double FOV, int screenWidth, int screenHeight);

// Iterates the player to move it with its current speeds
// Must be called every tick
void IteratePlayer(Player* player);

// Moves the player according to the speeds passed in 
// and delta time
void MovePlayer(Player* player, Speeds speed);

#endif

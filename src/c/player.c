#include "player.h"

#include <time.h>
#include <stdio.h>
#include <math.h>

#define BILLION 1000000000L

int tick = 0;

Speeds DecomposeSpeeds(Speeds initialSpeeds, double rot) {
	double vx = 0.0;
	double vy = 0.0;

	vy += initialSpeeds.vx * sin((rot / 180) * PI);
	vx += initialSpeeds.vx * cos((rot / 180) * PI);
	vy += initialSpeeds.vy * sin((rot / 180) * PI);
	vx += initialSpeeds.vy * cos((rot / 180) * PI);

	return (Speeds){.vx = vx, .vy = vy, .omega = initialSpeeds.omega};
}

void InitPlayer(Player* player) {
	player->x = 0;
	player->y = 0;
	player->rot = 0;
	player->_curSpeed = (Speeds){0, 0, 0};
	clock_gettime(CLOCK_REALTIME, &player->_curTime);
}

void IteratePlayer(Player* player) {
	struct timespec curTime;

	clock_gettime(CLOCK_REALTIME, &curTime);

	// In seconds
	double deltaTime = (curTime.tv_sec - player->_curTime.tv_sec) +
                  (curTime.tv_nsec - player->_curTime.tv_nsec) / (double)BILLION;

	// printf("%f\n", deltaTime);

	player->_curTime = curTime;

	Speeds decomposedSpeeds = DecomposeSpeeds(player->_curSpeed, player->rot);

	player->x += deltaTime * decomposedSpeeds.vx;
	player->y += deltaTime * decomposedSpeeds.vy;
	player->rot += deltaTime * player->_curSpeed.omega;

	tick++;

	// if (tick % 1000 == 0) {
	// 	printf("x: %f, y: %f, rot: %f\n", player->x, player->y, player->rot);
	// }
}

void MovePlayer(Player* player, Speeds speed) {
	player->_curSpeed = speed;
}

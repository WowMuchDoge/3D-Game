#include "player.h"

#include <time.h>
#include <stdio.h>

#define BILLION 1000000000L

int tick = 0;

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

	player->x += deltaTime * player->_curSpeed.vx;
	player->y += deltaTime * player->_curSpeed.vy;
	player->rot += deltaTime * player->_curSpeed.omega;

	tick++;

	if (tick % 1000 == 0) {
		printf("x: %f, y: %f, rot: %f\n", player->x, player->y, player->rot);
	}
}

void MovePlayer(Player* player, Speeds speed) {
	player->_curSpeed = speed;
}

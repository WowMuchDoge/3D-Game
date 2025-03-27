#include "polygon.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

static double degreeToRadian(double d) {
	return (d / 180) * PI;
}

static double radianToDegree(double r) {
	return (r / PI) * 180;
}

Polygon3d InitPolygon3d(Point3d points[], int size) {
	Polygon3d polygon;

	polygon.vertices = malloc(size * sizeof(Point3d));
	polygon.size = size;

	memcpy(polygon.vertices, points, size);

	return polygon;
}

void FreePolygon3d(Polygon3d polygon) {
	free(polygon.vertices);
}

Polygon2d InitPolygon2d(Point2d points[], int size) {
	Polygon2d polygon;

	polygon.vertices = malloc(size * sizeof(Point2d));
	polygon.size = size;

	memcpy(polygon.vertices, points, size);

	return polygon;
}

void FreePolygon2d(Polygon2d polygon) {
	free(polygon.vertices);
}

World InitWorld(Polygon3d polygons[], int size) {
	World world;

	world.worldPolygons = malloc(size * sizeof(Polygon3d));
	world.size = size;

	memcpy(world.worldPolygons, polygons, size);

	return world;
}

void FreeWorld(World world) {
	for (int i = 0; i < world.size; i++) {
		FreePolygon3d(world.worldPolygons[i]);
	}

	free(world.worldPolygons);
}

Point3d ToPlayerCoordinatePoint3d(Point3d point, Player player) {
	Point3d translatedPoint;

	double dx = point.x - player.x;
	double dy = point.y - player.y;
	double dz = point.z - PLAYER_CAMERA_HEIGHT;

	double distance = hypot(dx, dy);

	double theta = radianToDegree(acos(dy / distance)) - player.rot;

	translatedPoint.x = distance * sin(degreeToRadian(theta));
	translatedPoint.y = distance * cos(degreeToRadian(theta));
	translatedPoint.z = dz;

	return translatedPoint;
}

Polygon3d ToPlayerCoordinatePolygon3d(Polygon3d polygon, Player player) {
	Polygon3d translatedPolygon;

	translatedPolygon.vertices = malloc(polygon.size * sizeof(Polygon3d));
	translatedPolygon.size = polygon.size;

	for (int i = 0; i < polygon.size; i++) {
		translatedPolygon.vertices[i] = ToPlayerCoordinatePoint3d(polygon.vertices[i], player);
	}

	return translatedPolygon;
}

Point2d ProjectPointTo2d(Point3d point, Player player) {
	double realScreenWidth = player.screenWidth * PLAYER_METERS_PER_PIXEL;
	double realDistanceToFocalPoint = (realScreenWidth / 2) / (tan(degreeToRadian(player.FOV / 2)));

	// This ain't calculus but I don't want to think of a better name, d is for distance
	double dx = point.x - player.x;
	double dy = point.y - player.y;
	double dz = point.z - PLAYER_CAMERA_HEIGHT;

	// We use weak perspective projection
	double projectedX = (dx * realDistanceToFocalPoint) / dy;
	double projectedY = (dz * realDistanceToFocalPoint) / dy;

	return (Point2d){projectedX, projectedY};
}

Polygon2d ProjectPolygonTo2d(Polygon3d polygon, Player player) {
	Polygon2d projectedPoly;

	projectedPoly.vertices = malloc(polygon.size * sizeof(Polygon2d));
	projectedPoly.size = polygon.size;

	for (int i = 0; i < polygon.size; i++) {
		projectedPoly.vertices[i] = ProjectPointTo2d(polygon.vertices[i], player);
	}

	return projectedPoly;
}

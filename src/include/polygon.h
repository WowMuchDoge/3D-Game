#ifndef POLYGON_H_
#define POLYGON_H_

#include "player.h"

typedef struct {
	double x;
	double y;
} Point2d;

typedef struct {
	double x;
	double y;
	double z;
} Point3d;

typedef struct {
	Point3d* vertices;
	int size;
} Polygon3d;

typedef struct {
	Point2d* vertices;
	int size;
} Polygon2d;

typedef struct {
	Polygon3d* worldPolygons;
	int size;
} World;

// Initialized the points of a 3d polygon (points are
// rendered in order)
Polygon3d InitPolygon3d(Point3d points[], int size);

// Because space for points is dynamically allocated, you
// must free them to not get memory leaks
void FreePolygon3d(Polygon3d polygon);

// Initialized the points of a 2d polygon (points are
// rendered in order)
Polygon2d InitPolygon2d(Point2d points[], int size);

// Because space for points is dynamically allocated, you
// must free them to not get memory leaks
void FreePolygon2d(Polygon2d polygon);

// Returns an object containing an array of polygons with each
// containing polygons with corrdinates in world space
World InitWorld(Polygon3d polygons[], int size);

// Contains dynamically allocated memory so must be freed
void FreeWorld(World world);

// Creates a new world-relative point to a player-relative one
Point3d ToPlayerCoordinatePoint3d(Point3d coord, Player player);

// Creates a new polygon from world-coordinates to player-coordinate
Polygon3d ToPlayerCoordinatePolygon3d(Polygon3d polygon, Player player);

// Creates a new 2d point from a 3d point by projecting 3d space onto
// 2d space based on player properties (points must be player relative)
Point2d ProjectPointTo2d(Point3d point, Player player);

// Creates a new 2d polygon from a 3d polygon based on player
// properties (polygon must be player relative)
Polygon2d ProjectPolygonTo2d(Polygon3d polygon, Player player);

#endif

#include "gamelib.h"

#include <math.h>

Vec2 AddVec2(Vec2 a, Vec2 b) {
	a.x += b.x;
	a.y += b.y;
	return a;
}

Vec2 SubVec2(Vec2 a, Vec2 b) {
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

Vec2 ScaleVec2(Vec2 vec, int scalar) {
	vec.x *= scalar;
	vec.y *= scalar;
	return vec;
}

int Vec2Magnitude(Vec2 vec) {
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

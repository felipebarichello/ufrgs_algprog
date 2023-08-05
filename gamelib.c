#include "gamelib.h"

#include <math.h>

Vec2 Vec2_from_Vector2(Vector2 vector) {
	Vec2 vec;

	vec.x = (int)vector.x;
	vec.y = (int)vector.y;
	return vec;
}

char Vec2Equals(Vec2 a, Vec2 b) {
	return a.x == b.x && a.y == b.y;
}

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

Vec2 DivideVec2(Vec2 vec, int scalar) {
	vec.x /= scalar;
	vec.y /= scalar;
	return vec;
}

Vec2 NormalizeVec2(Vec2 vec) {
	return DivideVec2(vec, Vec2Magnitude(vec));
}

int Vec2Magnitude(Vec2 vec) {
	return (int)sqrt(vec.x * vec.x + vec.y * vec.y);
}

Vector2 Vector2_from_Vec2(Vec2 vec) {
	Vector2 vector;

	vector.x = (float)vec.x;
	vector.y = (float)vec.y;
	return vector;
}

Vector2 AddVector2(Vector2 a, Vector2 b) {
	a.x += b.x;
	a.y += b.y;
	return a;
}

Vector2 SubVector2(Vector2 a, Vector2 b) {
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

Vector2 ScaleVector2(Vector2 vec, float scalar) {
	vec.x *= scalar;
	vec.y *= scalar;
	return vec;
}

Vector2 DivideVector2(Vector2 vec, float scalar) {
	vec.x /= scalar;
	vec.y /= scalar;
	return vec;
}

Vector2 NormalizeVector2(Vector2 vec) {
	return DivideVector2(vec, Vector2Magnitude(vec));
}

float Vector2Magnitude(Vector2 vec) {
	return (float)sqrt(vec.x * vec.x + vec.y * vec.y);
}

float Vector2Angle(Vector2 vec) {
	return atan2f(vec.y, vec.x);
}


void BeginRotation(Vector2 translation, float angle) {
	rlPushMatrix();
	rlTranslatef(translation.x, translation.y, 0);
	rlRotatef(angle, 0, 0, 1);
}

void EndRotation() {
	rlPopMatrix();
}

#include "gamelib.h"

// Adicionar dois Vec2
Vec2 AddVec2(Vec2 a, Vec2 b) {
	a.x += b.x;
	a.y += b.y;
	return a;
}

// Subtrair o segundo Vec2 do primeiro
Vec2 SubVec2(Vec2 a, Vec2 b) {
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

// Multiplicar cada componente pela escalar
Vec2 ScaleVec2(Vec2 vec, int scalar) {
	vec.x *= scalar;
	vec.y *= scalar;
	return vec;
}

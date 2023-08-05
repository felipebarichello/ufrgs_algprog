/*
* Extensão da raylib
*/


#pragma once

#include "raylib.h"
#include "rlgl.h"

// Raíz quadrada de 2
#define SQRT_2 1.41421356237

typedef struct {
	int x, y;
} Vec2;

// Criar um Vector2 a partir de um Vec2
Vec2 Vec2_from_Vector2(Vector2);

// Adicionar dois Vec2
Vec2 AddVec2(Vec2, Vec2);

// Subtrair o segundo Vec2 do primeiro
Vec2 SubVec2(Vec2, Vec2);

// Multiplicar cada componente pela escalar
Vec2 ScaleVec2(Vec2, int);

// Dividir cada componente pela escalar
Vec2 DivideVec2(Vec2, int);

// Normalizar o vetor para magnitude 1
Vec2 NormalizeVec2(Vec2);

// Calcular a magnitude do vetor
int Vec2Magnitude(Vec2);

// Criar um Vector2 a partir de um Vec2
Vector2 Vector2_from_Vec2(Vec2);

// Adicionar dois Vector2
Vector2 AddVector2(Vector2, Vector2);

// Subtrair o segundo Vector2 do primeiro
Vector2 SubVector2(Vector2, Vector2);

// Multiplicar cada componente pela escalar
Vector2 ScaleVector2(Vector2, float);

// Dividir cada componente pela escalar
Vector2 DivideVector2(Vector2, float);

// Normalizar o vetor magnitude 1
Vector2 NormalizeVector2(Vector2);

// Calcular a magnitude do vetor
float Vector2Magnitude(Vector2);

float Vector2Angle(Vector2);


// Rotaciona tudo o que é desenhado antes de `EndRotation()`
// Deve sempre ser chamado em conjunto com `EndRotation()`, senão coisas ruins acontecem 😈
// O ângulo é em graus
// `translation` é a posição na tela
// Colocar uma posição diferente de zero em qualquer desenho significa um offset do PONTO DE ORIGEM
// Por exemplo, caso queira manter a origem de um retângulo, mas quer rotacioná-lo 45 graus para cima, faça assim:
//
// ```
// BeginRotation(position, -45);
//     DrawRectangle(0, 0, width, length, color);
// EndRotation();
// ```
void BeginRotation(Vector2 translation, float angle);

// Encerra a rotação
// Deve sempre ser chamada após `BeginRotation()`
void EndRotation();

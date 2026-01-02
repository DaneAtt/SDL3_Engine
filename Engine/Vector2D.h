#pragma once
#include "iostream"
#include "EngineAPI.h"

class ENGINE_API Vector2D
{
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& Add(const Vector2D& vec);
	Vector2D& Subtract(const Vector2D& vec);
	Vector2D& Multiply(const Vector2D& vec);
	Vector2D& Divide(const Vector2D& vec);

	friend ENGINE_API Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
	friend ENGINE_API Vector2D operator-(const Vector2D& v1, const Vector2D& v2);
	friend ENGINE_API Vector2D operator*(const Vector2D& v1, const Vector2D& v2);
	friend ENGINE_API Vector2D operator/(const Vector2D& v1, const Vector2D& v2);

	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);

	Vector2D& Zero();

	float Length();
	Vector2D& Normalize();

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);


};
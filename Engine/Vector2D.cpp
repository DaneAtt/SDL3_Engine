#include "Vector2D.h"

Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D& Vector2D::Add(const Vector2D& vec)
{
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2D& Vector2D::Subtract(const Vector2D& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2D& Vector2D::Multiply(const Vector2D& vec)
{
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

Vector2D& Vector2D::Divide(const Vector2D& vec)
{
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
{
	return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

Vector2D operator-(const Vector2D& v1, const Vector2D& v2)
{
	return Vector2D(v1.x - v2.x, v1.y - v2.y);
}

Vector2D operator*(const Vector2D& vec, float scalar)
{
	return Vector2D(vec.x * scalar, vec.y * scalar);
}

Vector2D operator*(float scalar, const Vector2D& vec)
{
	return Vector2D(vec.x * scalar, vec.y * scalar);
}

Vector2D operator/(const Vector2D& v1, const Vector2D& v2)
{
	return Vector2D(v1.x / v2.x, v1.y / v2.y);
}

Vector2D& Vector2D::operator+=(const Vector2D& vec)
{
	return this->Add(vec);

}

Vector2D& Vector2D::operator-=(const Vector2D& vec)
{
	return this->Subtract(vec);

}

Vector2D& Vector2D::operator*=(const Vector2D& vec)
{
	return this->Multiply(vec);

}

Vector2D& Vector2D::operator/=(const Vector2D& vec)
{
	return this->Divide(vec);

}

Vector2D& Vector2D::Zero()
{
	this->x = 0;
	this->y = 0;

	return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& vec)
{
	stream << "(" << vec.x << "," << vec.y << ")";
	return stream;
}

float Vector2D::Length()
{
	return sqrt(x * x + y * y);
}

Vector2D& Vector2D::Normalize()
{
	float length = Length();
	if (length != 0)  // or: if (length > 0.0001f) for floating point safety
	{
		x /= length;
		y /= length;
	}
	return *this;
}
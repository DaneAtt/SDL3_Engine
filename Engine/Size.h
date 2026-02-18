#pragma once

class Size
{
public:
	float w;
	float h;

	Size() : w(0), h(0) {}
	Size(float width, float height) : w(width), h(height) {}
};
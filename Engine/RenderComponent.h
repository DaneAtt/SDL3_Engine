#pragma once
#include "TextureManager.h"
#include "Engine.h"
#include "ECS.h"

class Component;

class RenderComponent : public Component
{
public:
	virtual void draw() = 0;

protected:
	TextureManager* texMan = Engine::getTextureManager();
};
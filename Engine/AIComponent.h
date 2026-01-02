#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "Vector2D.h"
#include "PathFinder.h"
#include <set>

class AIComponent : public Component
{
public:
	Entity* target = nullptr;

	enum State
	{
		Idle,
		Chase,
		Attack
	};

	State currentState;

	AIComponent()
	{
		currentState = State::Idle;
	}

	~AIComponent() {}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		position = transform->position;
		cellSizeX = Engine::getPathFinder()->getCellSizeX();
		cellSizeY = Engine::getPathFinder()->getCellSizeY();
	}

	void update() override
	{
		switch (currentState)
		{
		case State::Idle:
			break;
		case State::Chase:
			ChaseEntity(target);
			break;
		case State::Attack:
			break;
		}
		deltaTime = Engine::getDeltaTime();
	}

	void ChaseEntity(Entity* entity)
	{
		eTrans = &entity->getComponent<TransformComponent>();
		targetPos = eTrans->position;

		float distance = (targetPos - transform->position).Length();
		if (distance < 150.0f) {
			transform->velocity.Zero();
			return;
		}

		// Recalculate path every 0.5 seconds OR if we have no path
		pathUpdateTimer += Engine::getDeltaTime();
		if (pathUpdateTimer >= PATH_UPDATE_INTERVAL || currentPath.empty()) {
			pathUpdateTimer = 0.0f;
			updatePath(entity);
		}

		followPathSmooth();
	}

	void updatePath(Entity* target)
	{
		// Convert world positions to grid coordinates
		int srcRow = transform->position.y / cellSizeY;
		int srcCol = transform->position.x / cellSizeX;
		int destRow = target->getComponent<TransformComponent>().position.y / cellSizeY;
		int destCol = target->getComponent<TransformComponent>().position.x / cellSizeX;

		// Find path
		PathFinder::Pair src = std::make_pair(srcRow, srcCol);
		PathFinder::Pair dest = std::make_pair(destRow, destCol);

		std::vector<PathFinder::Pair> rawPath = Engine::getPathFinder()->findPath(src, dest);

		// Smooth the path
		if (!rawPath.empty()) {
			currentPath = Engine::getPathFinder()->smoothPath(rawPath);
		}
		else {
			currentPath.clear();
		}

		if (currentPath.empty()) {
			currentPathIndex = 0;
		}
	}

	void followPathSmooth()
	{
		if (currentPath.empty() || currentPathIndex >= currentPath.size()) {
			std::cout << "PATH IS EMPTY!" << std::endl;
			transform->velocity.Zero();
			return;
		}

		// Get current waypoint
		PathFinder::Pair waypoint = currentPath[currentPathIndex];

		// Convert to world coordinates (center of cell)
		Vector2D waypointWorld;
		waypointWorld.x = (waypoint.second * cellSizeX) + (cellSizeX / 2.0f);
		waypointWorld.y = (waypoint.first * cellSizeY) + (cellSizeY / 2.0f);

		// Calculate direction
		Vector2D direction = waypointWorld - transform->position;
		float distToWaypoint = direction.Length();

		// Close enough to waypoint?
		if (distToWaypoint < 10.0f) {
			currentPathIndex++;

			if (currentPathIndex >= currentPath.size()) {
				transform->velocity.Zero();
				currentPath.clear();
				currentPathIndex = 0;
				return;
			}
			return;
		}

		// Set velocity toward waypoint
		direction.Normalize();
		transform->velocity = direction;
	}

	void SetTarget(Entity* newTarget)
	{
		target = newTarget;
	}

private:
	TransformComponent* transform;
	TransformComponent* eTrans;
	float detectionRange;
	Vector2D position, targetPos;
	int speed;
	float deltaTime;

	std::vector<PathFinder::Pair> currentPath;
	int currentPathIndex = 0;
	float pathUpdateTimer = 0.0f;
	const float PATH_UPDATE_INTERVAL = 0.5f;  // Recalculate every 0.5 seconds
	int cellSizeX, cellSizeY;
};
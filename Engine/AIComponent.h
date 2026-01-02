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

		if (distance < 150.0f) {  // Adjust this value
			transform->velocity.Zero();
			return;
		}

		pathUpdateTimer += Engine::getDeltaTime();
		if (pathUpdateTimer >= PATH_UPDATE_INTERVAL || currentPath.empty()) {
			pathUpdateTimer = 0.0f;
			updatePath(entity);
		}

		followPath();
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
		currentPath = Engine::getPathFinder()->findPath(src, dest);
		currentPathIndex = 0;
	}

	void followPath()
	{
		if (currentPath.empty()) {
			transform->velocity.Zero();
			return;
		}

		// SAFETY CHECK 2: Index out of bounds
		if (currentPathIndex >= currentPath.size()) {
			transform->velocity.Zero();
			currentPath.clear();  // Clear the path
			currentPathIndex = 0;
			return;
		}

		// Get current waypoint (in grid coordinates)
		PathFinder::Pair waypoint = currentPath[currentPathIndex];

		// Convert to world coordinates (center of cell)
		Vector2D waypointWorld;
		waypointWorld.x = (waypoint.second * cellSizeX) + (cellSizeX / 2.0f);
		waypointWorld.y = (waypoint.first * cellSizeY) + (cellSizeY / 2.0f);

		// Move toward waypoint
		Vector2D direction = waypointWorld - transform->position;
		float distToWaypoint = direction.Length();

		// Reached waypoint? Move to next one
		if (distToWaypoint < std::min(cellSizeX, cellSizeY) / 2.0f) {
			currentPathIndex++;

			// Reached end of path?
			if (currentPathIndex >= currentPath.size()) {
				transform->velocity.Zero();
				currentPath.clear();
				currentPathIndex = 0;
				return;
			}
		}

		// Move toward current waypoint
		direction.Normalize();
		transform->velocity.x = direction.x;
		transform->velocity.y = direction.y;
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

	std::vector<PathFinder::Pair> currentPath;  // The path to follow
	int currentPathIndex = 0;  // Which waypoint we're heading to
	float pathUpdateTimer = 0.0f;  // When to recalculate path
	const float PATH_UPDATE_INTERVAL = 0.5f;  // Recalculate every 0.5 seconds
	int cellSizeX, cellSizeY;
};
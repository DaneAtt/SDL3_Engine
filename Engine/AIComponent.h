#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "Vector2D.h"
#include "PathFinder.h"
#include "EngineAPI.h"
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

    void ENGINE_API init() override;
    void ENGINE_API update() override;
    void ENGINE_API SetTarget(Entity* newTarget);

private:
    void ENGINE_API ChaseEntity(Entity* entity);
    void updatePath(Entity* target);
    void followPathSmooth();

    TransformComponent* transform = nullptr;
    TransformComponent* eTrans = nullptr;
    Vector2D targetPos;
    float deltaTime;

    std::vector<PathFinder::Pair> currentPath;
    int currentPathIndex = 0;
    float pathUpdateTimer = 0.0f;
    const float PATH_UPDATE_INTERVAL = 0.5f;
    const float TARGET_MOVE_THRESHOLD = 64.0f;  // Only repath if target moved this much
    const float WAYPOINT_REACH_DISTANCE = 40.0f;
    float smoothing = 0.18f; // Lower = smoother but slower turning
    int cellSizeX = 0, cellSizeY = 0;

    // Anti-jerk variables
    Vector2D lastTargetPos;
    Vector2D lastMyPos;
    int stuckCounter = 0;
};
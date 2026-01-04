#include "AIComponent.h"

void AIComponent::init()
{
    transform = &entity->getComponent<TransformComponent>();
    cellSizeX = Engine::getPathFinder()->getCellSizeX();
    cellSizeY = Engine::getPathFinder()->getCellSizeY();
}

void AIComponent::update()
{
    deltaTime = Engine::getDeltaTime();
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
}

void AIComponent::ChaseEntity(Entity* entity)
{
    if (!entity) return;

    eTrans = &entity->getComponent<TransformComponent>();
    targetPos = eTrans->position;

    float distance = (targetPos - transform->position).Length();

    if (distance < 150.0f) {
        transform->velocity.Zero();
        return;
    }

    pathUpdateTimer += deltaTime;

    // Only recalculate if:
    // 1. We have no path
    // 2. Timer expired AND target moved significantly
    // 3. We're stuck (not moving but should be)
    bool needsNewPath = currentPath.empty();

    if (!needsNewPath && pathUpdateTimer >= PATH_UPDATE_INTERVAL) {
        float targetMoved = (targetPos - lastTargetPos).Length();
        if (targetMoved > TARGET_MOVE_THRESHOLD) {
            needsNewPath = true;
        }

        // Check if stuck
        float myMovement = (transform->position - lastMyPos).Length();
        if (myMovement < 5.0f && currentPath.size() > 0) {
            stuckCounter++;
            if (stuckCounter > 0.3f) { // Stuck for too long
                needsNewPath = true;
                stuckCounter = 0;
            }
        }
        else {
            stuckCounter = 0;
        }
    }

    if (needsNewPath) {
        pathUpdateTimer = 0.0f;
        lastTargetPos = targetPos;
        lastMyPos = transform->position;
        updatePath(entity);
    }

    followPathSmooth();
}

void AIComponent::updatePath(Entity* target)
{
    // Convert world positions to grid coordinates
    int srcRow = static_cast<int>(transform->position.y) / cellSizeY;
    int srcCol = static_cast<int>(transform->position.x) / cellSizeX;
    int destRow = static_cast<int>(target->getComponent<TransformComponent>().position.y) / cellSizeY;
    int destCol = static_cast<int>(target->getComponent<TransformComponent>().position.x) / cellSizeX;

    if (PathFinder::PF_DEBUG) {
        std::cout << "[AI] world src=(" << transform->position.x << "," << transform->position.y << ") -> grid=("
            << srcRow << "," << srcCol << "), dest world=("
            << target->getComponent<TransformComponent>().position.x << "," << target->getComponent<TransformComponent>().position.y
            << ") -> grid=(" << destRow << "," << destCol << ")\n";
    }

    // Find path
    PathFinder::Pair src = std::make_pair(srcRow, srcCol);
    PathFinder::Pair dest = std::make_pair(destRow, destCol);

    std::vector<PathFinder::Pair> rawPath = Engine::getPathFinder()->findPath(src, dest);

    if (rawPath.empty()) {
        if (PathFinder::PF_DEBUG) std::cout << "[AI] rawPath empty\n";
        currentPath.clear();
        currentPathIndex = 0;
        return;
    }

    // Show raw path
    if (PathFinder::PF_DEBUG) {
        std::cout << "[AI] rawPath (len=" << rawPath.size() << "):";
        for (auto& p : rawPath) std::cout << " (" << p.first << "," << p.second << ")";
        std::cout << std::endl;
    }

    // Smooth the path
    currentPath = Engine::getPathFinder()->smoothPath(rawPath);

    // Show smoothed path
    if (PathFinder::PF_DEBUG) {
        std::cout << "[AI] smoothed path (len=" << currentPath.size() << "):";
        for (auto& p : currentPath) std::cout << " (" << p.first << "," << p.second << ")";
        std::cout << std::endl;
    }

    // Always start from index 0 on a new path
    currentPathIndex = 0;

    // Skip the first waypoint if we're already in that cell
    if (currentPath.size() > 1) {
        int myRow = static_cast<int>(transform->position.y) / cellSizeY;
        int myCol = static_cast<int>(transform->position.x) / cellSizeX;

        if (currentPath[0].first == myRow && currentPath[0].second == myCol) {
            currentPathIndex = 1;
        }
    }

    // Skip any additional waypoints we've already passed
    for (size_t i = currentPathIndex; i < currentPath.size(); i++)
    {
        Vector2D waypointWorld;
        waypointWorld.x = (currentPath[i].second * cellSizeX) + (cellSizeX / 2.0f);
        waypointWorld.y = (currentPath[i].first * cellSizeY) + (cellSizeY / 2.0f);

        float dist = (waypointWorld - transform->position).Length();
        if (dist < WAYPOINT_REACH_DISTANCE) {
            currentPathIndex = static_cast<int>(i) + 1;
        }
    }

    if (PathFinder::PF_DEBUG) {
        std::cout << "[AI] starting at path index: " << currentPathIndex << "\n";
    }
}

void AIComponent::followPathSmooth()
{
    if (currentPath.empty() || currentPathIndex >= static_cast<int>(currentPath.size())) {
        transform->velocity.Zero();
        return;
    }

    // Get current waypoint in world coords
    PathFinder::Pair waypoint = currentPath[currentPathIndex];
    Vector2D waypointWorld;
    waypointWorld.x = (waypoint.second * cellSizeX) + (cellSizeX / 2.0f);
    waypointWorld.y = (waypoint.first * cellSizeY) + (cellSizeY / 2.0f);

    Vector2D toWaypoint = waypointWorld - transform->position;
    float distToWaypoint = toWaypoint.Length();

    // Advance waypoint if close enough
    while (distToWaypoint < WAYPOINT_REACH_DISTANCE && currentPathIndex < static_cast<int>(currentPath.size()) - 1) {
        currentPathIndex++;
        waypoint = currentPath[currentPathIndex];
        waypointWorld.x = (waypoint.second * cellSizeX) + (cellSizeX / 2.0f);
        waypointWorld.y = (waypoint.first * cellSizeY) + (cellSizeY / 2.0f);
        toWaypoint = waypointWorld - transform->position;
        distToWaypoint = toWaypoint.Length();
    }

    // Final waypoint reached?
    if (currentPathIndex >= static_cast<int>(currentPath.size()) - 1 && distToWaypoint < WAYPOINT_REACH_DISTANCE) {
        transform->velocity.Zero();
        currentPath.clear();
        currentPathIndex = 0;
        return;
    }

    // Calculate desired direction
    Vector2D desiredDir = toWaypoint;
    desiredDir.Normalize();

    // Smooth rotation, not velocity magnitude
    Vector2D currentDir = transform->velocity;
    float currentSpeed = currentDir.Length();

    if (currentSpeed > 0.01f) {
        currentDir.Normalize();
        // Blend directions
        Vector2D newDir = currentDir * (1.0f - smoothing) + desiredDir * smoothing;
        newDir.Normalize();
        transform->velocity = newDir;
    }
    else {
        // Not moving yet, just set direction directly
        transform->velocity = desiredDir;
    }
}

void AIComponent::SetTarget(Entity* newTarget)
{
    target = newTarget;
    lastTargetPos = Vector2D(0, 0);
    currentPath.clear();
    currentPathIndex = 0;
}
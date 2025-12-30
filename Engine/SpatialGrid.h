#pragma once
#include "SDL3/SDL.h"
#include <vector>
#include <unordered_map>
#include "EngineAPI.h"

class CollisionComponent;
class HitboxComponent;

class ENGINE_API SpatialGrid
{
public:
    SpatialGrid() {};
    SpatialGrid(int worldWidth, int worldHeight);
    ~SpatialGrid();
    void Insert(CollisionComponent* obj, SDL_FRect* rect);
    void Remove(CollisionComponent* obj);
    void Update(CollisionComponent* obj, SDL_FRect* rect);
    std::vector<CollisionComponent*> Query(SDL_FRect* rect);

    std::vector<std::vector<std::vector<CollisionComponent*>>> getCollisionGrid() { return cells; }

private:
    int cellWidth, cellHeight;
    int numCellsX, numCellsY;
    std::vector<std::vector<std::vector<CollisionComponent*>>> cells;
    std::unordered_map<CollisionComponent*, std::vector<std::pair<int, int>>> objectCells;
};
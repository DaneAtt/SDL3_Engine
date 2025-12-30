#include "SpatialGrid.h"
#include "CollisionComponent.h"
#include "Engine.h"
#include "WindowRender.h"
#include <unordered_set>

SpatialGrid::SpatialGrid(int worldWidth, int worldHeight) 
{
    const int defaultCellSize = 64;
    cellWidth = std::max(16, defaultCellSize);
    cellHeight = std::max(16, defaultCellSize);

    // derive number of cells from world size
    numCellsX = std::max(1, (worldWidth + cellWidth - 1) / cellWidth);
    numCellsY = std::max(1, (worldHeight + cellHeight - 1) / cellHeight);

    // Initialize 2D grid: cells[y][x] stores objects in that cell
    cells.resize(numCellsY);
    for (int y = 0; y < numCellsY; y++) {
        cells[y].resize(numCellsX);  // each inner vector is initially empty
    }
    // After this, each cells[y][x] is an empty vector of CollisionComponent*


}

SpatialGrid::~SpatialGrid() {}

void SpatialGrid::Insert(CollisionComponent* obj, SDL_FRect* rect)
{
    int leftCellX = static_cast<int>(rect->x) / cellWidth;
    int rightCellX = static_cast<int>((rect->x + rect->w)) / cellWidth; // rightmost cell that the rect touches
    int topCellY = static_cast<int>(rect->y) / cellHeight;
    int bottomCellY = static_cast<int>((rect->y + rect->h)) / cellHeight; // bottommost cell that the rect touches

    // Clamp(bounds checking)
    leftCellX = std::max(0, std::min(leftCellX, numCellsX - 1));
    rightCellX = std::max(0, std::min(rightCellX, numCellsX - 1));
    topCellY = std::max(0, std::min(topCellY, numCellsY - 1));
    bottomCellY = std::max(0, std::min(bottomCellY, numCellsY - 1));

    objectCells[obj].clear();

    for (int y = topCellY; y <= bottomCellY; y++) {
        for (int x = leftCellX; x <= rightCellX; x++) {
            cells[y][x].push_back(obj);
            objectCells[obj].push_back({ x, y });
        }
    }
}

void SpatialGrid::Remove(CollisionComponent* obj)
{
    if (objectCells.find(obj) != objectCells.end()) {
        for (auto& [x, y] : objectCells[obj]) {
            auto& cell = cells[y][x];
            auto curOjbect = std::find(cell.begin(), cell.end(), obj);
            if (curOjbect != cell.end()) {
                cell.erase(curOjbect);
            }
        }
    }
    objectCells.erase(obj);
}

void SpatialGrid::Update(CollisionComponent* obj, SDL_FRect* rect)
{
    Remove(obj);
    Insert(obj, rect);
}

std::vector<CollisionComponent*> SpatialGrid::Query(SDL_FRect* rect)
{
    int leftCellX = static_cast<int>(rect->x) / cellWidth;
    int rightCellX = static_cast<int>((rect->x + rect->w)) / cellWidth; // rightmost cell that the rect touches
    int topCellY = static_cast<int>(rect->y) / cellHeight;
    int bottomCellY = static_cast<int>((rect->y + rect->h)) / cellHeight; // bottommost cell that the rect touches

    // Clamp(bounds checking)
    leftCellX = std::max(0, std::min(leftCellX, numCellsX - 1));
    rightCellX = std::max(0, std::min(rightCellX, numCellsX - 1));
    topCellY = std::max(0, std::min(topCellY, numCellsY - 1));
    bottomCellY = std::max(0, std::min(bottomCellY, numCellsY - 1));

    std::unordered_set<CollisionComponent*> seen;
    std::vector<CollisionComponent*> result;
    result.reserve((rightCellX - leftCellX + 1) * (bottomCellY - topCellY + 1) * 2);

    // Deduplicate objects that appear in multiple cells
    for (int y = topCellY; y <= bottomCellY; y++) {
        for (int x = leftCellX; x <= rightCellX; x++) {
            for (auto* obj : cells[y][x]) {
                if (seen.insert(obj).second) {
                    result.push_back(obj);
                }
            }
        }
    }
    return result;
}
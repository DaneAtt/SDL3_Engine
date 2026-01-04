#pragma once
#include <queue>
#include <vector>
#include <cmath>
#include <stack>
#include <utility>
#include <cfloat>
#include <iostream>
#include "EngineAPI.h"

class Map; // Forward declaration

class ENGINE_API PathFinder {
public:
    typedef std::pair<int, int> Pair;
    typedef std::pair<double, Pair> pPair;

    // Structure to hold cell details
    struct cell {
        int parent_i, parent_j;
        double f, g, h;
    };

    static constexpr bool PF_DEBUG = false;

    PathFinder(int mapWidth, int mapHeight, int cellSizeX, int cellSizeY);
    ~PathFinder() {};

    void buildCollisionGrid(Map& map, std::string layerID, int inflateRadius = 0);
    std::vector<Pair> findPath(Pair src, Pair dest) { return aStarSearch(src, dest); }
    std::vector<Pair> smoothPath(const std::vector<Pair>& path);
    bool hasLineOfSight(Pair a, Pair b);
    bool isWalkableAt(int row, int col);
    std::vector<Pair> tracePath(std::vector<std::vector<cell>>& cellDetails, Pair dest);
    double calculateHValue(int row, int col, Pair dest);
    std::vector<Pair> aStarSearch(Pair src, Pair dest);
    Pair findNearestWalkable(Pair target);

    // Getters
    int getCellSizeX() const { return cellSizeX; }
    int getCellSizeY() const { return cellSizeY; }
    int getRows() const { return rows; }
    int getCols() const { return cols; }

private:
    struct compare {
        bool operator()(const pPair& a, const pPair& b) { return a.first > b.first; }
    };

    // Private methods
    bool isValid(int row, int col);
    bool isUnBlocked(int row, int col);
    bool isDestination(int row, int col, Pair dest);

    // Member variables
    int rows, cols, cellSizeX, cellSizeY;
    std::vector<std::vector<bool>> collisionGrid;
};
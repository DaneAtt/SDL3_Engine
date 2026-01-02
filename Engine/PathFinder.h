#pragma once
#include <queue>
#include <vector>
#include <cmath>
#include <stack>
#include <utility>
#include <cfloat>
#include "Map.h"

class PathFinder {
public:
    // Shortcut for row,col pair
    typedef std::pair<int, int> Pair;

    // Shortcut for f-value and cell coordinates
    typedef std::pair<double, Pair> pPair;

    // Structure to hold cell details
    struct cell {
        int parent_i, parent_j; // Parent cell coordinates
        double f, g, h;         // f = g + h
    };

    // Constructor: map dimensions and cell sizes
    PathFinder(int mapWidth, int mapHeight, int cellSizeX, int cellSizeY) : cellSizeX(cellSizeX), cellSizeY(cellSizeY)
    {
        rows = mapHeight / cellSizeY;
        cols = mapWidth / cellSizeX;
        collisionGrid.resize(rows, std::vector<bool>(cols, true));
    }

    // Build collision grid from map layer
    void buildCollisionGrid(Map& map, std::string layerID) {
        // Reset grid to all walkable
        for (int i = 0;i < rows;i++)
            for (int j = 0;j < cols;j++)
                collisionGrid[i][j] = true;

        auto& collisionTiles = map.searchMap(layerID);

        // Mark tiles as blocked
        for (const auto& tile : collisionTiles) {
            int row = tile.yPos / cellSizeY;
            int col = tile.xPos / cellSizeX;
            if (isValid(row, col)) collisionGrid[row][col] = false;
        }
    }

    std::vector<Pair> smoothPath(const std::vector<Pair>& path) {
        if (path.size() < 3) return path;  // Too short to smooth

        std::vector<Pair> smoothed;
        smoothed.push_back(path[0]);  // Always keep start

        int i = 0;
        while (i < path.size() - 1) {
            int farthest = i + 1;  // Default: next cell

            // Try to see how far we can "see" from current position
            for (int j = i + 2; j < path.size(); j++) {
                if (hasLineOfSight(path[i], path[j])) {
                    farthest = j;  // Can see this far, keep going
                }
                else {
                    break;  // Can't see further, stop
                }
            }

            smoothed.push_back(path[farthest]);  // Add farthest visible point
            i = farthest;  // Jump to that point
        }

        return smoothed;
    }

    // Check if there's a clear line of sight between two cells
    bool hasLineOfSight(Pair a, Pair b) {
        // Bresenham's line algorithm - draws a line between two points
        int x0 = a.second;  // Start column
        int y0 = a.first;   // Start row
        int x1 = b.second;  // End column
        int y1 = b.first;   // End row

        int dx = abs(x1 - x0);  // Horizontal distance
        int dy = abs(y1 - y0);  // Vertical distance

        int sx = (x0 < x1) ? 1 : -1;  // Step direction X (left or right)
        int sy = (y0 < y1) ? 1 : -1;  // Step direction Y (up or down)

        int err = dx - dy;  // Error term for line drawing

        // Walk along the line
        while (true) {
            // Check if this cell is blocked
            if (!isWalkableAt(y0, x0)) {
                return false;  // Hit a wall, can't see through
            }

            // Reached the end?
            if (x0 == x1 && y0 == y1) {
                return true;  // Clear line of sight!
            }

            // Move to next cell along the line
            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x0 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y0 += sy;
            }
        }
    }

    // Helper: check if a cell is walkable (needed for line of sight)
    bool isWalkableAt(int row, int col) {
        if (!isValid(row, col)) return false;  // Out of bounds
        return collisionGrid[row][col];        // true = walkable
    }

    // Main pathfinding function
    std::vector<Pair> findPath(Pair src, Pair dest) {
        return aStarSearch(src, dest);
    }

    int getCellSizeX() const { return cellSizeX; }
    int getCellSizeY() const { return cellSizeY; }

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }

private:
    int rows, cols, cellSizeX, cellSizeY;
    std::vector<std::vector<bool>> collisionGrid;

    // Check if cell is inside grid
    bool isValid(int row, int col) { return row >= 0 && row < rows && col >= 0 && col < cols; }

    // Check if cell is walkable
    bool isUnBlocked(int row, int col) { return collisionGrid[row][col]; }

    // Check if cell is destination
    bool isDestination(int row, int col, Pair dest) { return row == dest.first && col == dest.second; }

    // Heuristic: Euclidean distance
    double calculateHValue(int row, int col, Pair dest) {
        return sqrt((row - dest.first) * (row - dest.first) + (col - dest.second) * (col - dest.second));
    }

    // Trace path from destination to source using parent pointers
    std::vector<Pair> tracePath(std::vector<std::vector<cell>>& cellDetails, Pair dest) {
        std::stack<Pair> Path;
        int row = dest.first;   // current row, start at destination
        int col = dest.second;  // current column, start at destination

        // Follow parent pointers until reaching the start cell
        while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col)) {
            Path.push({ row, col }); // push current cell onto path stack

            int temp_row = cellDetails[row][col].parent_i; // parent row of current cell
            int temp_col = cellDetails[row][col].parent_j; // parent column of current cell

            row = temp_row; // move to parent row
            col = temp_col; // move to parent column
        }

        // push the starting cell
        Path.push({ row, col }); // row/col now at source

        // Convert the stack into a vector (path from start to end)
        std::vector<Pair> path;
        while (!Path.empty()) {
            Pair p = Path.top();    // get the next cell from stack
            Path.pop();             // remove it from stack
            path.push_back(p);      // add to path vector
        }
        return path;
    }

    // Comparator for priority queue (min f-value at top)
    struct compare {
        bool operator()(const pPair& a, const pPair& b) { return a.first > b.first; }
    };

    // The A* search algorithm
    std::vector<Pair> aStarSearch(Pair src, Pair dest) {
        std::vector<Pair> emptyPath;

        // 1.0 Validate source and destination
        if (!isValid(src.first, src.second) || !isValid(dest.first, dest.second)) return emptyPath;
        if (!isUnBlocked(src.first, src.second) || !isUnBlocked(dest.first, dest.second)) return emptyPath;
        if (isDestination(src.first, src.second, dest)) return emptyPath;

        // 2.0 Closed list: tracks explored cells
        std::vector<std::vector<bool>> closedList(rows, std::vector<bool>(cols, false));

        // 3.0 Cell details: store f, g, h and parent
        std::vector<std::vector<cell>> cellDetails(rows, std::vector<cell>(cols));

        // 4.0 Initialize all cells
        for (int i = 0;i < rows;i++)
            for (int j = 0;j < cols;j++) {
                cellDetails[i][j].f = cellDetails[i][j].g = cellDetails[i][j].h = FLT_MAX;
                cellDetails[i][j].parent_i = cellDetails[i][j].parent_j = -1;
            }

        // 5.0 Initialize source cell
        int i = src.first, j = src.second;
        cellDetails[i][j].f = cellDetails[i][j].g = cellDetails[i][j].h = 0.0;
        cellDetails[i][j].parent_i = i;
        cellDetails[i][j].parent_j = j;

        // 6.0 Open list: priority queue sorted by f-value
        std::priority_queue<pPair, std::vector<pPair>, compare> openList;
        openList.push({ 0.0,{i,j} });

        // 7.0 Movement directions (N,S,E,W + diagonals)
        int rowDir[8] = { -1, 1, 0, 0, -1, -1, 1, 1 };
        int colDir[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };
        double moveCost[8] = { 1,1,1,1,1.414,1.414,1.414,1.414 }; // diagonal = sqrt(2)

        // 8.0 Main loop: process cells until destination is found or open list is empty
        while (!openList.empty()) {
            // Get cell with lowest f-value from open list
            auto p = openList.top(); openList.pop();
            i = p.second.first;   // current row
            j = p.second.second;  // current column

            if (closedList[i][j]) continue; // skip if already evaluated

            closedList[i][j] = true; // mark current cell as evaluated

            // 9.0 Explore all 8 neighbors
            for (int dir = 0; dir < 8; dir++) {
                int ni = i + rowDir[dir]; // neighbor row
                int nj = j + colDir[dir]; // neighbor column

                if (!isValid(ni, nj)) continue;      // skip invalid cells
                if (!isUnBlocked(ni, nj)) continue;  // skip blocked cells

                if (isDestination(ni, nj, dest)) {  // found destination
                    cellDetails[ni][nj].parent_i = i; // set parent row
                    cellDetails[ni][nj].parent_j = j; // set parent col
                    return tracePath(cellDetails, dest);
                }

                if (!closedList[ni][nj]) {
                    double gNew = cellDetails[i][j].g + moveCost[dir];   // cost from start
                    double hNew = calculateHValue(ni, nj, dest);        // heuristic to destination
                    double fNew = gNew + hNew;                          // total cost

                    if (cellDetails[ni][nj].f == FLT_MAX || cellDetails[ni][nj].f > fNew) {
                        openList.push({ fNew, {ni, nj} });              // push neighbor to open list
                        cellDetails[ni][nj].f = fNew;                 // update f-value
                        cellDetails[ni][nj].g = gNew;                 // update g-value
                        cellDetails[ni][nj].h = hNew;                 // update h-value
                        cellDetails[ni][nj].parent_i = i;             // set parent row
                        cellDetails[ni][nj].parent_j = j;             // set parent col
                    }
                }
            }
        }

        // 10.0 Destination not reached
        return emptyPath;
    }
};

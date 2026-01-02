#pragma once
#include <cmath>        // For sqrt() in heuristic
#include <set>          // For open list
#include <utility>      // For pair
#include <stack>        // For path reconstruction
#include <vector>       // For returning path
#include <cfloat>       // For FLT_MAX
#include <cstring>      // For memset (or just avoid memset)
#include <string>
#include "Map.h"

class PathFinder {
public:
    // Creating a shortcut for int, int pair type
    typedef std::pair<int, int> Pair;

    // Creating a shortcut for pair<int, pair<int, int>> type
    typedef std::pair<double, std::pair<int, int> > pPair;

    // A structure to hold the necessary parameters
    struct cell {
        // Row and Column index of its parent
        // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
        int parent_i, parent_j;
        // f = g + h
        double f, g, h;
    };

    PathFinder(int mapWidth, int mapHeight, int cellSizeX, int cellSizeY) {
        this->cellSizeX = cellSizeX;
        this->cellSizeY = cellSizeY;
        this->cols = mapWidth / cellSizeX;
        this->rows = mapHeight / cellSizeY;

        collisionGrid.resize(rows, std::vector<bool>(cols, true));
    }

    std::vector<Pair> findPath(Pair src, Pair dest) {
        return aStarSearch(src, dest);
    }

    void buildCollisionGrid(Map& map, std::string layerID) {
        // Reset grid to all walkable
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                collisionGrid[i][j] = true;  // true = walkable
            }
        }

        std::vector<Map::TileData>& collisionTiles = map.searchMap(layerID);

        // Mark tiles as blocked
        for (const auto& tile : collisionTiles) {
            int col = tile.xPos / cellSizeX;
            int row = tile.yPos / cellSizeY;

            if (isValid(row, col)) {
                collisionGrid[row][col] = false;  // false = blocked
            }
        }
    }

    int getCellSizeX() const { return cellSizeX; }
    int getCellSizeY() const { return cellSizeY; }

    bool isWalkableAt(int row, int col) {
        if (!isValid(row, col)) return false;
        return collisionGrid[row][col];  // true = walkable, false = blocked
    }

    int getRows() const {
        return rows;
    }

    int getCols() const {
        return cols;
    }

private:
    int rows, cols, cellSizeX, cellSizeY;
    std::vector<std::vector<bool>> collisionGrid;
    // A Utility Function to check whether given cell (row, col)
    // is a valid cell or not.
    bool isValid(int row, int col) {
        // Returns true if row number and column number
        // is in range
        return (row >= 0) && (row < rows) && (col >= 0) && (col < cols);
    }

    // A Utility Function to check whether the given cell is
    // blocked or not
    bool isUnBlocked(int row, int col)
    {
        // Check the pre-built collision grid
        return collisionGrid[row][col];
    }

    // A Utility Function to check whether destination cell has
    // been reached or not
    bool isDestination(int row, int col, Pair dest)
    {
        if (row == dest.first && col == dest.second)
            return (true);
        else
            return (false);
    }

    // A Utility Function to calculate the 'h' heuristics.
    double calculateHValue(int row, int col, Pair dest)
    {
        // Return using the distance formula
        return ((double)sqrt((row - dest.first) * (row - dest.first)
            + (col - dest.second) * (col - dest.second)));
    }

    // A Utility Function to trace the path from the source
    // to destination
    std::vector<Pair> tracePath(std::vector<std::vector<cell>>& cellDetails, Pair dest)
    {
        std::vector<Pair> path;
        int row = dest.first;
        int col = dest.second;

        std::stack<Pair> Path;

        while (!(cellDetails[row][col].parent_i == row
            && cellDetails[row][col].parent_j == col)) {
            Path.push(std::make_pair(row, col));
            int temp_row = cellDetails[row][col].parent_i;
            int temp_col = cellDetails[row][col].parent_j;
            row = temp_row;
            col = temp_col;
        }

        Path.push(std::make_pair(row, col));

        // Convert stack to vector
        while (!Path.empty()) {
            path.push_back(Path.top());
            Path.pop();
        }

        return path;
    }

    // A Function to find the shortest path between
    // a given source cell to a destination cell according
    // to A* Search Algorithm
    std::vector<Pair> aStarSearch(Pair src, Pair dest)
    {   
        std::vector<Pair> emptyPath;
        // 1.0 Validation
        // If the source is out of range
        if (isValid(src.first, src.second) == false) {
            printf("Source is invalid\n");
            return emptyPath;
        }

        // If the destination is out of range
        if (isValid(dest.first, dest.second) == false) {
            printf("Destination is invalid\n");
            return emptyPath;
        }

        // Either the source or the destination is blocked
        if (isUnBlocked(src.first, src.second) == false
            || isUnBlocked(dest.first, dest.second)
            == false) {
            printf("Source or the destination is blocked\n");
            return emptyPath;
        }

        // If the destination cell is the same as source cell
        if (isDestination(src.first, src.second, dest)
            == true) {
            printf("We are already at the destination\n");
            return emptyPath;
        }

        // 2.0 Create a closed list and initialise it to false which
        // means that no cell has been included yet This closed
        // list is implemented as a boolean 2D Vector
        std::vector<std::vector<bool>> closedList(rows, std::vector<bool>(cols, false));

        // Declare a 2D array of structure to hold the details
        // of that Vector
        std::vector<std::vector<cell>> cellDetails(rows, std::vector<cell>(cols));

        int i, j;
        // Initialize all cells to "unknown" (FLT_MAX = infinity)
        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                cellDetails[i][j].f = FLT_MAX; // Don't know cost yet
                cellDetails[i][j].g = FLT_MAX;
                cellDetails[i][j].h = FLT_MAX;
                cellDetails[i][j].parent_i = -1; // No parent yet
                cellDetails[i][j].parent_j = -1;
            }
        }

        // Set up starting cell
        i = src.first, j = src.second;
        cellDetails[i][j].f = 0.0; // Distance from start to start = 0
        cellDetails[i][j].g = 0.0; // We'll calculate this properly later
        cellDetails[i][j].h = 0.0;
        cellDetails[i][j].parent_i = i; // Start is its own parent
        cellDetails[i][j].parent_j = j;

        /*
         4. Create an open list having information as-
         <f, <i, j>>
         where f = g + h,
         and i, j are the row and column index of that cell
         Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
         This open list is implemented as a set of pair of
         pair.*/
        std::set<pPair> openList;

        // Put the starting cell on the open list and set its
        // 'f' as 0
        openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

        // We set this boolean value as false as initially
        // the destination is not reached.
        bool foundDest = false;

        // 5. MAIN LOOP - explore cells until we find destination
        while (!openList.empty()) {
            // Get cell with LOWEST f-value (most promising)
            pPair p = *openList.begin();
            openList.erase(openList.begin()); // Remove it from open list

            // Add this vertex to the closed list
            i = p.second.first; // Row
            j = p.second.second; // Column
            closedList[i][j] = true; // Mark as explored

            /*
             Generating all the 8 successor of this cell

                 N.W   N   N.E
                   \   |   /
                    \  |  /
                 W----Cell----E
                      / | \
                    /   |  \
                 S.W    S   S.E

             Cell-->Popped Cell (i, j)
             N -->  North       (i-1, j)
             S -->  South       (i+1, j)
             E -->  East        (i, j+1)
             W -->  West        (i, j-1)
             N.E--> North-East  (i-1, j+1)
             N.W--> North-West  (i-1, j-1)
             S.E--> South-East  (i+1, j+1)
             S.W--> South-West  (i+1, j-1)*/

             // To store the 'g', 'h' and 'f' of the 8 successors
            double gNew, hNew, fNew;

            //----------- 1st Successor (North) ------------

            // Only process this cell if this is a valid one
            if (isValid(i - 1, j) == true) {
                // If the destination cell is the same as the
                // current successor
                if (isDestination(i - 1, j, dest) == true) {
                    cellDetails[i - 1][j].parent_i = i;
                    cellDetails[i - 1][j].parent_j = j;
                    foundDest = true;
                    return tracePath(cellDetails, dest);
                }
                // Not destination, check if we should explore it
                else if (closedList[i - 1][j] == false // Not explored yet
                    && isUnBlocked(i - 1, j) == true) { // Not blocked
                    // Calculate costs
                    gNew = cellDetails[i][j].g + 1.0; // +1 for cardinal direction
                    hNew = calculateHValue(i - 1, j, dest); // Euclidean distance
                    fNew = gNew + hNew;

                    // If it isn’t on the open list, add it to
                    // the open list. Make the current square
                    // the parent of this square. Record the
                    // f, g, and h costs of the square cell
                    //                OR
                    // If it is on the open list already, check
                    // to see if this path to that square is
                    // better, using 'f' cost as the measure.
                    if (cellDetails[i - 1][j].f == FLT_MAX // Never visited
                        || cellDetails[i - 1][j].f > fNew) { // Or found better path

                        // Add to open list
                        openList.insert(std::make_pair(
                            fNew, std::make_pair(i - 1, j)));

                        // Update the details of this cell
                        cellDetails[i - 1][j].f = fNew;
                        cellDetails[i - 1][j].g = gNew;
                        cellDetails[i - 1][j].h = hNew;
                        cellDetails[i - 1][j].parent_i = i; // Current cell is parent
                        cellDetails[i - 1][j].parent_j = j;
                    }
                }
            }

            //----------- 2nd Successor (South) ------------

            if (isValid(i + 1, j) == true) {
                if (isDestination(i + 1, j, dest) == true) {
                    cellDetails[i + 1][j].parent_i = i;
                    cellDetails[i + 1][j].parent_j = j;
                    foundDest = true;
                    return tracePath(cellDetails, dest);
                }
                else if (closedList[i + 1][j] == false
                    && isUnBlocked(i + 1, j)
                    == true) {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i + 1, j, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i + 1][j].f == FLT_MAX
                        || cellDetails[i + 1][j].f > fNew) {
                        openList.insert(std::make_pair(
                            fNew, std::make_pair(i + 1, j)));

                        cellDetails[i + 1][j].f = fNew;
                        cellDetails[i + 1][j].g = gNew;
                        cellDetails[i + 1][j].h = hNew;
                        cellDetails[i + 1][j].parent_i = i;
                        cellDetails[i + 1][j].parent_j = j;
                    }
                }
            }

            //----------- 3rd Successor (East) ------------

            if (isValid(i, j + 1) == true) {
                if (isDestination(i, j + 1, dest) == true) {
                    cellDetails[i][j + 1].parent_i = i;
                    cellDetails[i][j + 1].parent_j = j;
                    foundDest = true;
                    return tracePath(cellDetails, dest);
                }

                else if (closedList[i][j + 1] == false
                    && isUnBlocked(i, j + 1)
                    == true) {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i, j + 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i][j + 1].f == FLT_MAX
                        || cellDetails[i][j + 1].f > fNew) {
                        openList.insert(std::make_pair(
                            fNew, std::make_pair(i, j + 1)));

                        cellDetails[i][j + 1].f = fNew;
                        cellDetails[i][j + 1].g = gNew;
                        cellDetails[i][j + 1].h = hNew;
                        cellDetails[i][j + 1].parent_i = i;
                        cellDetails[i][j + 1].parent_j = j;
                    }
                }
            }

            //----------- 4th Successor (West) ------------

            if (isValid(i, j - 1) == true) {
                if (isDestination(i, j - 1, dest) == true) {
                    cellDetails[i][j - 1].parent_i = i;
                    cellDetails[i][j - 1].parent_j = j;
                    foundDest = true;
                    return tracePath(cellDetails, dest);
                }

                else if (closedList[i][j - 1] == false
                    && isUnBlocked(i, j - 1)
                    == true) {
                    gNew = cellDetails[i][j].g + 1.0;
                    hNew = calculateHValue(i, j - 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i][j - 1].f == FLT_MAX
                        || cellDetails[i][j - 1].f > fNew) {
                        openList.insert(std::make_pair(
                            fNew, std::make_pair(i, j - 1)));

                        cellDetails[i][j - 1].f = fNew;
                        cellDetails[i][j - 1].g = gNew;
                        cellDetails[i][j - 1].h = hNew;
                        cellDetails[i][j - 1].parent_i = i;
                        cellDetails[i][j - 1].parent_j = j;
                    }
                }
            }

            //----------- 5th Successor (North-East) ------------

            if (isValid(i - 1, j + 1) == true) {
                if (isDestination(i - 1, j + 1, dest) == true
                    && isUnBlocked(i - 1, j) == true
                    && isUnBlocked(i, j + 1) == true) {
                    cellDetails[i - 1][j + 1].parent_i = i;
                    cellDetails[i - 1][j + 1].parent_j = j;
                    foundDest = true;
                    return tracePath(cellDetails, dest);
                }

                else if (closedList[i - 1][j + 1] == false
                    && isUnBlocked(i - 1, j + 1)== true
                    && isUnBlocked(i - 1, j) == true
                    && isUnBlocked(i, j + 1) == true){
                    gNew = cellDetails[i][j].g + 1.414;
                    hNew = calculateHValue(i - 1, j + 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i - 1][j + 1].f == FLT_MAX
                        || cellDetails[i - 1][j + 1].f > fNew) {
                        openList.insert(std::make_pair(
                            fNew, std::make_pair(i - 1, j + 1)));

                        cellDetails[i - 1][j + 1].f = fNew;
                        cellDetails[i - 1][j + 1].g = gNew;
                        cellDetails[i - 1][j + 1].h = hNew;
                        cellDetails[i - 1][j + 1].parent_i = i;
                        cellDetails[i - 1][j + 1].parent_j = j;
                    }
                }
            }

            //----------- 6th Successor (North-West) -----------

            if (isValid(i - 1, j - 1) == true) {
                if (isDestination(i - 1, j - 1, dest) == true
                    && isUnBlocked(i - 1, j) == true
                    && isUnBlocked(i, j - 1) == true) {
                    cellDetails[i - 1][j - 1].parent_i = i;
                    cellDetails[i - 1][j - 1].parent_j = j;
                    foundDest = true;
                    return tracePath(cellDetails, dest);
                }

                else if (closedList[i - 1][j - 1] == false
                    && isUnBlocked(i - 1, j - 1)== true
                    && isUnBlocked(i - 1, j) == true
                    && isUnBlocked(i, j - 1) == true) {
                    gNew = cellDetails[i][j].g + 1.414;
                    hNew = calculateHValue(i - 1, j - 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i - 1][j - 1].f == FLT_MAX
                        || cellDetails[i - 1][j - 1].f > fNew) {
                        openList.insert(std::make_pair(
                            fNew, std::make_pair(i - 1, j - 1)));
                        cellDetails[i - 1][j - 1].f = fNew;
                        cellDetails[i - 1][j - 1].g = gNew;
                        cellDetails[i - 1][j - 1].h = hNew;
                        cellDetails[i - 1][j - 1].parent_i = i;
                        cellDetails[i - 1][j - 1].parent_j = j;
                    }
                }
            }

            //----------- 7th Successor (South-East) -----------

            if (isValid(i + 1, j + 1) == true) {
                if (isDestination(i + 1, j + 1, dest) == true
                    && isUnBlocked(i + 1, j) == true
                    && isUnBlocked(i, j + 1) == true) {
                    cellDetails[i + 1][j + 1].parent_i = i;
                    cellDetails[i + 1][j + 1].parent_j = j;
                    foundDest = true;
                    return tracePath(cellDetails, dest);
                }

                else if (closedList[i + 1][j + 1] == false
                    && isUnBlocked(i + 1, j + 1)== true
                    && isUnBlocked(i + 1, j) == true
                    && isUnBlocked(i, j + 1) == true) {
                    gNew = cellDetails[i][j].g + 1.414;
                    hNew = calculateHValue(i + 1, j + 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i + 1][j + 1].f == FLT_MAX
                        || cellDetails[i + 1][j + 1].f > fNew) {
                        openList.insert(std::make_pair(
                            fNew, std::make_pair(i + 1, j + 1)));

                        cellDetails[i + 1][j + 1].f = fNew;
                        cellDetails[i + 1][j + 1].g = gNew;
                        cellDetails[i + 1][j + 1].h = hNew;
                        cellDetails[i + 1][j + 1].parent_i = i;
                        cellDetails[i + 1][j + 1].parent_j = j;
                    }
                }
            }

            //----------- 8th Successor (South-West) -----------

            if (isValid(i + 1, j - 1) == true) {
                if (isDestination(i + 1, j - 1, dest) == true
                    && isUnBlocked(i + 1, j) == true
                    && isUnBlocked(i, j - 1) == true) {
                    cellDetails[i + 1][j - 1].parent_i = i;
                    cellDetails[i + 1][j - 1].parent_j = j;
                    foundDest = true;
                    return tracePath(cellDetails, dest);
                }

                else if (closedList[i + 1][j - 1] == false
                    && isUnBlocked(i + 1, j - 1)== true
                    && isUnBlocked(i + 1, j) == true
                    && isUnBlocked(i, j - 1) == true) {
                    gNew = cellDetails[i][j].g + 1.414;
                    hNew = calculateHValue(i + 1, j - 1, dest);
                    fNew = gNew + hNew;

                    if (cellDetails[i + 1][j - 1].f == FLT_MAX
                        || cellDetails[i + 1][j - 1].f > fNew) {
                        openList.insert(std::make_pair(
                            fNew, std::make_pair(i + 1, j - 1)));

                        // Update the details of this cell
                        cellDetails[i + 1][j - 1].f = fNew;
                        cellDetails[i + 1][j - 1].g = gNew;
                        cellDetails[i + 1][j - 1].h = hNew;
                        cellDetails[i + 1][j - 1].parent_i = i;
                        cellDetails[i + 1][j - 1].parent_j = j;
                    }
                }
            }
        }

        // When the destination cell is not found and the open
        // list is empty, then we conclude that we failed to
        // reach the destination cell. This may happen when the
        // there is no way to destination cell (due to
        // blockages)
        if (foundDest == false)
            printf("Failed to find the Destination Cell\n");
        return emptyPath;
    }
};
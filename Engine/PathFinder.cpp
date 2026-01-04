#include "PathFinder.h"
#include "Map.h"

using Pair = PathFinder::Pair;

PathFinder::PathFinder(int mapWidth, int mapHeight, int cellSizeX, int cellSizeY) : cellSizeX(cellSizeX), cellSizeY(cellSizeY)
{
    rows = mapHeight / cellSizeY;
    cols = mapWidth / cellSizeX;
    collisionGrid.resize(rows, std::vector<bool>(cols, true));
}

// Build collision grid from map layer 
void PathFinder::buildCollisionGrid(Map& map, std::string layerID, int inflateRadius)
{
    // Reset grid to all walkable
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            collisionGrid[i][j] = true;

    auto& collisionTiles = map.searchMap(layerID);

    // Mark tiles as blocked
    for (const auto& tile : collisionTiles) {
        int row = tile.yPos / cellSizeY;
        int col = tile.xPos / cellSizeX;

        // Mark the actual tile as blocked
        if (isValid(row, col)) {
            collisionGrid[row][col] = false;
        }

        // Inflate: also mark neighbors as blocked for pathfinding
        // This keeps entity CENTER away from walls
        for (int dr = -inflateRadius; dr <= inflateRadius; dr++) {
            for (int dc = -inflateRadius; dc <= inflateRadius; dc++) {
                int nr = row + dr;
                int nc = col + dc;
                if (isValid(nr, nc)) {
                    collisionGrid[nr][nc] = false;
                }
            }
        }
    }
}

std::vector<Pair> PathFinder::smoothPath(const std::vector<Pair>& path) {
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
        }

        smoothed.push_back(path[farthest]);  // Add farthest visible point
        i = farthest;  // Jump to that point
    }

    return smoothed;
}

bool PathFinder::hasLineOfSight(Pair a, Pair b)
{
    int x0 = a.second, y0 = a.first;
    int x1 = b.second, y1 = b.first;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x1 > x0) ? 1 : -1;
    int sy = (y1 > y0) ? 1 : -1;

    int err = dx - dy;

    int x = x0;
    int y = y0;

    while (true)
    {
        // Check if entity can fit at this position
        if (!isUnBlocked(y, x))
            return false;

        if (x == x1 && y == y1)
            return true;

        int e2 = err * 2;

        int prevX = x;
        int prevY = y;

        if (e2 > -dy)
        {
            err -= dy;
            x += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y += sy;
        }

        // SUPER-COVER diagonal handling
        if (x != prevX && y != prevY)
        {
            if (!isUnBlocked(prevY, x) || !isUnBlocked(y, prevX))
                return false;
        }
    }
}

// Helper: check if a cell is walkable (needed for line of sight)
bool PathFinder::isWalkableAt(int row, int col) {
    if (!isValid(row, col)) return false;
    return collisionGrid[row][col];
}

// Check if cell is inside grid
bool PathFinder::isValid(int row, int col) { return row >= 0 && row < rows && col >= 0 && col < cols; }

bool PathFinder::isUnBlocked(int row, int col) { return collisionGrid[row][col]; }

// Check if cell is destination
bool PathFinder::isDestination(int row, int col, Pair dest) { return row == dest.first && col == dest.second; }


Pair PathFinder::findNearestWalkable(Pair target)
{
    // If target is already walkable, return it
    if (isValid(target.first, target.second) && isUnBlocked(target.first, target.second)) {
        return target;
    }

    // BFS to find nearest walkable cell
    std::queue<Pair> queue;
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    queue.push(target);
    if (isValid(target.first, target.second)) {
        visited[target.first][target.second] = true;
    }

    int rowDir[8] = { -1, 1, 0, 0, -1, -1, 1, 1 };
    int colDir[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };

    while (!queue.empty()) {
        Pair current = queue.front();
        queue.pop();

        for (int d = 0; d < 8; d++) {
            int nr = current.first + rowDir[d];
            int nc = current.second + colDir[d];

            if (!isValid(nr, nc)) continue;
            if (visited[nr][nc]) continue;

            visited[nr][nc] = true;

            if (isUnBlocked(nr, nc)) {
                return { nr, nc };  // Found nearest walkable
            }

            queue.push({ nr, nc });
        }
    }

    // No walkable cell found (shouldn't happen in normal maps)
    return target;
}

// Heuristic: Octile distance
double PathFinder::calculateHValue(int row, int col, Pair dest)
{
    double dRow = abs(row - dest.first);   // vertical cell distance
    double dCol = abs(col - dest.second);  // horizontal cell distance

    // Convert to world units
    double dy = dRow * cellSizeY;
    double dx = dCol * cellSizeX;

    // Octile for non-square: take diagonals, then straight remainder
    double diagonalCost = sqrt(static_cast<double>(cellSizeX * cellSizeX + cellSizeY * cellSizeY));
    double diagonalSteps = std::min(dRow, dCol);
    double straightSteps = std::max(dRow, dCol) - diagonalSteps;

    // Cost = diagonal steps * diagonal cost + remaining straight steps * their cost
    if (dRow > dCol) {
        // More vertical movement remaining
        return diagonalSteps * diagonalCost + straightSteps * cellSizeY;
    }
    else {
        // More horizontal movement remaining
        return diagonalSteps * diagonalCost + straightSteps * cellSizeX;
    }
}

// Trace path from destination to source using parent pointers
std::vector<Pair> PathFinder::tracePath(std::vector<std::vector<cell>>& cellDetails, Pair dest) {
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
    if (PF_DEBUG) {
        std::cout << "[PF] traced path (length=" << path.size() << "):";
        for (auto& p : path) std::cout << " (" << p.first << "," << p.second << ")";
        std::cout << std::endl;
    }
    return path;
}

// The A* search algorithm
std::vector<Pair> PathFinder::aStarSearch(Pair src, Pair dest) {
    std::vector<Pair> emptyPath;
    if (PF_DEBUG) {
        std::cout << "[PF] findPath src=(" << src.first << "," << src.second << ") dest=("
            << dest.first << "," << dest.second << ")\n";
    }
    // 1.0 Validate source and destination
    if (!isValid(src.first, src.second) || !isValid(dest.first, dest.second)) {
        if (PF_DEBUG) std::cout << "[PF] invalid src/dest grid coords\n";
        return emptyPath;
    }

    // If destination is blocked, find nearest walkable cell instead
    if (!isUnBlocked(dest.first, dest.second)) {
        Pair newDest = findNearestWalkable(dest);
        if (PF_DEBUG) {
            std::cout << "[PF] dest blocked, rerouting from (" << dest.first << "," << dest.second
                << ") to (" << newDest.first << "," << newDest.second << ")\n";
        }
        dest = newDest;
    }

    // Temporarily unblock source if entity is standing in blocked cell
    bool srcWasBlocked = !isUnBlocked(src.first, src.second);
    if (srcWasBlocked) {
        collisionGrid[src.first][src.second] = true;
    }
    if (isDestination(src.first, src.second, dest)) {
        if (PF_DEBUG) std::cout << "[PF] src is dest\n";
        return emptyPath;
    }

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

    double verticalCost = static_cast<double>(cellSizeY);
    double horizontalCost = static_cast<double>(cellSizeX);
    double diagonalCost = sqrt(static_cast<double>(cellSizeX * cellSizeX + cellSizeY * cellSizeY));

    double moveCost[8] =
    {
        verticalCost, verticalCost,           // up, down
        horizontalCost, horizontalCost,       // right, left
        diagonalCost, diagonalCost, diagonalCost, diagonalCost  // diagonals
    };

    // 8.0 Main loop: process cells until destination is found or open list is empty
    while (!openList.empty()) {
        // Get cell with lowest f-value from open list
        auto p = openList.top(); openList.pop();
        i = p.second.first;   // current row
        j = p.second.second;  // current column

        if (closedList[i][j]) {
            if (PF_DEBUG) std::cout << "[PF] popped already-closed (" << i << "," << j << ")\n";
            continue;
        }

        closedList[i][j] = true; // mark current cell as evaluated

        // 9.0 Explore all 8 neighbors
        for (int dir = 0; dir < 8; dir++) {
            int ni = i + rowDir[dir]; // neighbor row
            int nj = j + colDir[dir]; // neighbor column

            if (!isValid(ni, nj)) continue;      // skip invalid cells
            if (!isUnBlocked(ni, nj)) continue;  // skip blocked cells

            if (!isValid(ni, nj)) {
                if (PF_DEBUG) std::cout << "[PF] neighbor out of bounds (" << ni << "," << nj << ")\n";
                continue;
            }

            if (!isUnBlocked(ni, nj)) {
                if (PF_DEBUG) std::cout << "[PF] neighbor blocked (" << ni << "," << nj << ")\n";
                continue;
            }

            // Prevent diagonal corner-cutting:
            // if moving diagonally, ensure both adjoining orthogonal cells are unblocked.
            if (dir >= 4) { // indices 4..7 are diagonals in our array
                int orth1_i = i;      int orth1_j = nj; // horizontal neighbor
                int orth2_i = ni;     int orth2_j = j;  // vertical neighbor

                if (!isUnBlocked(orth1_i, orth1_j) || !isUnBlocked(orth2_i, orth2_j)) {
                    if (PF_DEBUG) {
                        std::cout << "[PF] diagonal corner-cut blocked at (" << ni << "," << nj << ")\n";
                    }
                    continue;
                }
            }

            if (isDestination(ni, nj, dest)) {  // found destination
                cellDetails[ni][nj].parent_i = i; // set parent row
                cellDetails[ni][nj].parent_j = j; // set parent col
                if (PF_DEBUG) std::cout << "[PF] destination found at (" << ni << "," << nj << ")\n";
                if (srcWasBlocked) {
                    collisionGrid[src.first][src.second] = false;
                }
                return tracePath(cellDetails, dest);
            }

            if (!closedList[ni][nj]) {
                double gNew = cellDetails[i][j].g + moveCost[dir];   // cost from start
                double hNew = calculateHValue(ni, nj, dest);        // heuristic to destination
                double fNew = gNew + hNew;                          // total cost

                if (cellDetails[ni][nj].f == FLT_MAX || cellDetails[ni][nj].f > fNew) {
                    if (PF_DEBUG) {
                        std::cout << "[PF] push neighbor (" << ni << "," << nj << ") f=" << fNew
                            << " g=" << gNew << " h=" << hNew << "\n";
                    }
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
    if (srcWasBlocked) {
        collisionGrid[src.first][src.second] = false;
    }

    // 10.0 Destination not reached
    if (PF_DEBUG) std::cout << "[PF] destination not reached, returning empty path\n";
    if (PF_DEBUG)
    {
        std::cout << "=== PATH FAILURE DEBUG ===" << std::endl;
        std::cout << "Trying to reach dest: (" << dest.first << ", " << dest.second << ")" << std::endl;
        std::cout << "Dest isValid: " << isValid(dest.first, dest.second) << std::endl;
        std::cout << "Dest isUnBlocked: " << isUnBlocked(dest.first, dest.second) << std::endl;

        // Show 5x5 grid around destination
        std::cout << "Grid around dest (. = walkable, # = blocked, D = dest):" << std::endl;
        for (int r = dest.first - 2; r <= dest.first + 2; r++) {
            for (int c = dest.second - 2; c <= dest.second + 2; c++) {
                if (r == dest.first && c == dest.second) {
                    std::cout << "D";
                }
                else if (!isValid(r, c)) {
                    std::cout << "X";
                }
                else if (!isUnBlocked(r, c)) {
                    std::cout << "#";
                }
                else {
                    std::cout << ".";
                }
            }
            std::cout << std::endl;
        }
        std::cout << "==========================" << std::endl;
    }
    return emptyPath;
}

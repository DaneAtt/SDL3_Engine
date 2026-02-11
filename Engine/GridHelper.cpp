#include "GridHelper.h"

GridHelper::CellPosition GridHelper::calculateCellPosition(int row, int col)
{
    CellPosition pos;
    float scale = getInventoryScale();

    float scaledCell = getCellSize() * scale;
    float scaledGapH = getHorizontalGap() * scale;
    float scaledGapV = getVerticalGap() * scale;

    pos.x = getGridStartX() * scale + col * (scaledCell + scaledGapH);
    pos.y = getGridStartY() * scale + row * (scaledCell + scaledGapV);
    pos.w = scaledCell;
    pos.h = scaledCell;

    return pos;
}

void GridHelper::populateGrid()
{
    mainPanel->clearWidgets();
    gridPanel.clear();
    gridPanel.resize(getGridRows());

    for (int row = 0; row < getGridRows(); row++)
    {
        for (int col = 0; col < getGridCols(); col++)
        {
            CellPosition cell = calculateCellPosition(row, col);

            Panel& cellPanel = mainPanel->addWidget<Panel>(
                Vector2D(cell.x, cell.y),
                UISize(cell.w, cell.h),
                SDL_Color{ 49, 49, 49, 255 }
            );

            gridPanel[row].push_back(&cellPanel);
        }
    }
}

void GridHelper::trackGridHover()
{
    Vector2D mouse = Engine::getMouse();
    hoveredRow = -1;
    hoveredCol = -1;

    for (int row = 0; row < gridPanel.size(); row++)
    {
        for (int col = 0; col < gridPanel[row].size(); col++)
        {
            if (gridPanel[row][col]->containsPoint(mouse))
            {
                hoveredRow = row;
                hoveredCol = col;
                break;
            }
        }
        if (hoveredRow != -1) break; // Break outer loop
    }

    // Now update colors based on hoveredRow/hoveredCol
}
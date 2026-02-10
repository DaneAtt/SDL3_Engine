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

            float screenX = getScreenUIX() + cell.x;
            float screenY = getScreenUIY() + cell.y;

            Panel& cellPanel = mainPanel->addWidget<Panel>(
                Vector2D(screenX, screenY),
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
            CellPosition cell = calculateCellPosition(row, col);
            float screenX = getScreenUIX() + cell.x;
            float screenY = getScreenUIY() + cell.y;

            if (mouse.x >= screenX && mouse.x <= screenX + cell.w &&
                mouse.y >= screenY && mouse.y <= screenY + cell.h)
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
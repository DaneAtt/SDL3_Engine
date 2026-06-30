#pragma once
#include "EngineAPI.h"
#include "Widget.h"
#include <memory>
#include <unordered_map>
#include "TextureManager.h"
#include "Size.h"
#include "LayoutHelperBase.h"

class ENGINE_API GridHelper : public LayoutHelperBase
{
protected:
	struct CellPosition
	{
        float x, y, w, h;
	};

    CellPosition calculateCellPosition(int row, int col);
    void populateGrid();
    void trackGridHover();
    void playEffectAt(int row, int col, const char* animName);

    virtual float getCellSize() const = 0;
    virtual float getHorizontalGap() const = 0;
    virtual float getVerticalGap() const = 0;
    virtual int getGridRows() const = 0;
    virtual int getGridCols() const = 0;

	std::vector<std::vector<Panel*>> gridPanel;
    std::vector<std::vector<Panel*>> cellAnimations;
    int hoveredRow = -1;
    int hoveredCol = -1;
    int lastHoveredRow = -1;
};
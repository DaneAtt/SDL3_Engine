#pragma once
#include "EngineAPI.h"
#include "Widget.h"
#include <memory>
#include <unordered_map>
#include "TextureManager.h"
#include "Size.h"

class ENGINE_API GridHelper
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

    virtual float getGridStartX() const = 0;
    virtual float getGridStartY() const = 0;
    virtual float getCellSize() const = 0;
    virtual float getHorizontalGap() const = 0;
    virtual float getVerticalGap() const = 0;
    virtual int getGridRows() const = 0;
    virtual int getGridCols() const = 0;
    virtual float getScreenUIX() const = 0;
    virtual float getScreenUIY() const = 0;
    virtual int getInventoryScale() const = 0;

    TextureManager* texManager = Engine::getTextureManager();
	std::unique_ptr<Panel> mainPanel;
	std::vector<std::vector<Panel*>> gridPanel;
    std::vector<std::vector<Panel*>> cellAnimations;
    int hoveredRow = -1;
    int hoveredCol = -1;
    int lastHoveredRow = -1;
};
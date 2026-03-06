#pragma once
#include "EngineAPI.h"
#include "Widget.h"
#include "Size.h"
#include "TextureManager.h"

class ENGINE_API ScrollableListHelper 
{
protected:
	struct PanelLocation
	{
		float x, y, w, h;
	};

	struct RowAnimState 
	{
		float currentY;
		float targetY;
	};

	PanelLocation& calculateStartLocation(PanelLocation& l)
	{
		int inventoryScale = getInventoryScale();
		l.x = getPanelStartX() * inventoryScale;
		l.y = getPanelStartY() * inventoryScale;
		l.w = getPanelWidth() * inventoryScale;
		l.h = getPanelHeight() * inventoryScale;
		return l;
	}

	void rebuildVisibleRows(Panel* mainPanel, std::vector<Panel*>& rowPanels);
	void updateRowHover(int& lastHoveredRow, std::vector<Panel*>& rowPanels);
	void checkScroll(SDL_Event& event);
	int getClickedSlot(Vector2D& pos);

	void updateRowAnimation(int sourceLocal, int sourceIndex);
	void trackDragDirection(int sourceIndex, int hoverIndex);  // calculates targetY
	void tickRowAnimations(int index);   // lerps currentY toward targetY each frame
	void drawHighlight();

	virtual int getRowCount() const = 0;
	virtual void populateRow(Panel& row, int index, const PanelLocation& loc, float yPos) = 0;
	virtual float getPanelStartX() const = 0;
	virtual float getPanelStartY() const = 0;
	virtual float getPanelWidth() const = 0;
	virtual float getPanelHeight() const = 0;
	virtual float getScreenUIX() const = 0;
	virtual float getScreenUIY() const = 0;
	virtual int getInventoryScale() const = 0;

	// Shared members
	std::unique_ptr<Panel> mainPanel;
	std::vector<Panel*> rowPanels;
	std::vector<RowAnimState> rowAnimStates;
	std::vector<float> originalY;
	int scrollOffset = 0;
	int visibleRows = 16;
	int hoverLocal = -1;
	TextureManager* texManager = Engine::getTextureManager();
};
#pragma once
#include "EngineAPI.h"
#include "Widget.h"
#include "Size.h"
#include "TextureManager.h"
#include "LayoutHelperBase.h"

class ENGINE_API ScrollableListHelper : public LayoutHelperBase
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

	void rebuildVisibleRows(Panel* mainPanel);
	void updateRowHover(int& lastHoveredRow);
	void checkScroll(SDL_Event& event);
	int getClickedSlot(Vector2D& pos);

	void updateRowAnimation(int sourceLocal, int sourceIndex);
	void trackDragDirection(int sourceIndex, int hoverIndex);  // calculates targetY
	void tickRowAnimations(int index);   // lerps currentY toward targetY each frame
	void drawHighlight();

	virtual int getRowCount() const = 0;
	virtual void populateRow(Panel& row, int index, const PanelLocation& loc, float yPos) = 0;
	virtual float getPanelWidth() const = 0;
	virtual float getPanelHeight() const = 0;

	// Shared members
	std::vector<Panel*> rowPanels;
	std::vector<RowAnimState> rowAnimStates;
	std::vector<float> originalY;
	int scrollOffset = 0;
	int visibleRows = 16;
	int hoverLocal = -1;
};
#pragma once
#include "EngineAPI.h"
#include "Widget.h"

class ENGINE_API ScrollableListHelper 
{
protected:
	struct PanelLocation
	{
		float x, y, w, h;
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
	int scrollOffset = 0;
	int visibleRows = 0;

};
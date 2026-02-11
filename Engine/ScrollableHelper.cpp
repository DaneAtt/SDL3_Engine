#include "ScrollableHelper.h"
#include "Widget.h"
#include "Vector2D.h"
#include "SDL3/SDL.h"
#include "UIElement.h"


void ScrollableListHelper::rebuildVisibleRows(Panel* mainPanel, std::vector<Panel*>& rowPanels)
{
	mainPanel->clearWidgets();
	rowPanels.clear();

	PanelLocation loc;
	loc = calculateStartLocation(loc);
	int endIndex = std::min(scrollOffset + visibleRows, getRowCount());

	for (int i = scrollOffset; i < endIndex; i++) {
		float yPos = loc.y + (i - scrollOffset) * loc.h;
		Panel& rowPanel = mainPanel->addWidget<Panel>(
			Vector2D(loc.x, yPos),
			UISize(loc.w, loc.h),
			SDL_Color{ 49, 49, 49, 255 }
		);
		rowPanels.push_back(&rowPanel);

		populateRow(rowPanel, i, loc, yPos);
	}
}

void ScrollableListHelper::updateRowHover(int& lastHoveredRow, std::vector<Panel*>& rowPanels)
{
	Vector2D mouse = Engine::getMouse();

	PanelLocation loc;
	loc = calculateStartLocation(loc);
	int endIndex = std::min(scrollOffset + visibleRows, getRowCount());

	int hoveredRow = -1;
	for (int i = 0; i < rowPanels.size(); ++i)
	{
		if (rowPanels[i]->containsPoint(mouse))
		{
			hoveredRow = i;
			break;
		}
	}

	// Only update if hover changed
	if (hoveredRow != lastHoveredRow)
	{
		// Unhover old row
		if (lastHoveredRow >= 0 && lastHoveredRow < rowPanels.size())
		{
			rowPanels[lastHoveredRow]->setColor(SDL_Color{ 49, 49, 49, 255 });
		}

		// Hover new row
		if (hoveredRow >= 0 && hoveredRow < rowPanels.size())
		{
			rowPanels[hoveredRow]->setColor(SDL_Color{ 100, 100, 150, 255 });
		}

		lastHoveredRow = hoveredRow;
	}
}

void ScrollableListHelper::checkScroll(SDL_Event& event)
{
	if (event.wheel.y > 0) // Scrolled Up
	{
		scrollOffset--;
		scrollOffset = std::max(0, scrollOffset);
	}
	else if (event.wheel.y < 0) // Scrolled Down
	{
		scrollOffset++;
		int maxScroll = std::max(0, getRowCount() - visibleRows);
		scrollOffset = std::min(scrollOffset, maxScroll);
	}
}
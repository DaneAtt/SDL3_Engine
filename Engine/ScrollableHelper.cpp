#include "ScrollableHelper.h"
#include "Widget.h"
#include "Vector2D.h"
#include "SDL3/SDL.h"
#include "UIElement.h"

void ScrollableListHelper::rebuildVisibleRows(Panel* mainPanel)
{
	mainPanel->clearWidgets();
	rowPanels.clear();
	originalY.clear();
	rowAnimStates.clear();

	PanelLocation loc;
	loc = calculateStartLocation(loc);
	int endIndex = std::min(scrollOffset + visibleRows, getRowCount());

	for (int i = scrollOffset; i < endIndex; i++) {
		float yPos = loc.y + (i - scrollOffset) * loc.h;
		originalY.push_back(yPos); // save the ypos for panel dragging animation
		rowAnimStates.push_back({ yPos, yPos });  // currentY and targetY both start at yPos
		Panel& rowPanel = mainPanel->addWidget<Panel>(
			Vector2D(loc.x, yPos),
			Size(loc.w, loc.h),
			SDL_Color{ 49, 49, 49, 255 }
		);
		rowPanels.push_back(&rowPanel);

		populateRow(rowPanel, i, loc, yPos);
	}
}

void ScrollableListHelper::updateRowHover(int& lastHoveredRow)
{
	Vector2D mouse = Engine::getMouse();

	PanelLocation loc;
	loc = calculateStartLocation(loc);

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

int ScrollableListHelper::getClickedSlot(Vector2D& position)
{
	PanelLocation loc;
	loc = calculateStartLocation(loc);
	for (size_t i = 0; i < rowPanels.size(); i++)
	{
		float top = getScreenUIY() + originalY[i];
		float bottom = top + loc.h;
		float left = getScreenUIX() + loc.x;
		float right = left + loc.w;

		if (position.x >= left && position.x <= right &&
			position.y >= top && position.y <= bottom)
		{
			return scrollOffset + i;
		}
	}
	return -1;
}

void ScrollableListHelper::trackDragDirection(int sourceIndex, int hoverIndex)
{
	for (int i = 0; i < rowPanels.size(); i++)
	{
		if (i == sourceIndex)
		{
			continue;
		}

		if (sourceIndex < hoverIndex) //dragging downwards
		{
			if (i > sourceIndex && i <= hoverIndex)
				rowAnimStates[i].targetY = originalY[i - 1];
			else
				rowAnimStates[i].targetY = originalY[i];
		}
		else // dragging upwards
		{
			if (i >= hoverIndex && i < sourceIndex)
				rowAnimStates[i].targetY = originalY[i + 1];
			else
				rowAnimStates[i].targetY = originalY[i];
		}
	}
}

void ScrollableListHelper::tickRowAnimations(int index)
{
	for (int i = 0; i < rowPanels.size(); i++)
	{
		if (index != -1 && i == index)
		{
			rowPanels[i]->setVisibility(false);
			continue;
		}

		auto& state = rowAnimStates[i];
		state.currentY += (state.targetY - state.currentY) * 10.0f * Engine::getDeltaTime();
		rowPanels[i]->setPosition({ rowPanels[i]->getPosition().x, state.currentY });
	}
}

void ScrollableListHelper::updateRowAnimation(int sourceLocal, int sourceIndex)
{
	Vector2D& mouse = Engine::getMouse();
	int hoverIndex = getClickedSlot(mouse);	
	sourceLocal = sourceIndex - scrollOffset;
	hoverLocal = hoverIndex - scrollOffset;
	if (hoverIndex != -1)
		trackDragDirection(sourceLocal, hoverLocal);
	else hoverLocal = -1;

	tickRowAnimations(sourceLocal);
}

void ScrollableListHelper::drawHighlight()
{
	PanelLocation loc;
	loc = calculateStartLocation(loc);

	SDL_FRect highlight = {
		getScreenUIX() + loc.x,
		getScreenUIY() + originalY[hoverLocal],
		loc.w,
		loc.h
	};
	texManager->DrawRectFCombined(&highlight, { 74, 227, 232, 255 }, { 129, 165, 166, 150 });
}
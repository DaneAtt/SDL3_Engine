#pragma once
#include <vector>
#include <memory>
#include "Vector2D.h"
#include "UIElement.h"
#include "SDL3/SDL.h"

template<typename ElementType>
class UIContainer
{
public:
    UIContainer() {};
    ~UIContainer() {};

    template<typename T, typename... TArgs>
    T& add(TArgs&&... mArgs) 
    {
        T* element = new T(std::forward<TArgs>(mArgs)...);

        element->setParent(owner);

        std::unique_ptr<ElementType> uPtr{ element };
        elements.emplace_back(std::move(uPtr));
        element->init();
        return *element;
    }

    void init()
    {
        for (auto& e : elements) e->init();
    }

    void render()
    {
        for (auto& e : elements) e->render();
    }

    void update()
    {
        for (auto& e : elements) e->update();
    }

    void handleInput(SDL_Event& event, Vector2D& mouse) 
    {
        for (auto& e : elements) e->handleInput(event, mouse);
    }

    void handleClick(Vector2D& mouse)
    {
        for (auto& e : elements)
        {
            if (e->containsPoint(mouse))
            {
                e->handleClick(mouse);
            }
        }
    }

    bool containsPoint(Vector2D& mouse)
    {
        for (auto& e : elements) 
        {
            if(e->containsPoint(mouse)) return true;
        }
        return false;
    }

    void setOwner(UIElement* o) { owner = o; }

    void clear() { elements.clear(); }

private:
    std::vector<std::unique_ptr<ElementType>> elements;
    UIElement* owner = nullptr;
    
};

/*
1. uiManager->render()

2. UIManager::container.render() (loops elements)

3. for each element: UIElement::render() (returns early if visible==false)

4. InventoryUI::renderC() (because InventoryUI is one of those elements)

5. InventoryUI updates positions/sizes, then calls mainContainer.render()

6. mainContainer loops its elements and calls UIElement::render() on each (Panels, Buttons, Labels)

7. a Panel's renderC() draws the panel then calls its own container.render() to draw labels/buttons inside it

8. Label::renderC() does the actual text draw
*/
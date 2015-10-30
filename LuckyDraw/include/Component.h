#pragma once

#include "SceneNode.h"

#include <memory>

namespace sf
{
class Event;
class RenderWindow;
}

namespace GUI
{

class Component : public SceneNode
{
    public:
        typedef std::shared_ptr<Component> Ptr;

    public:
        Component() = default;
        virtual ~Component(){}

        virtual uint16_t getWidth() const = 0;
        virtual uint16_t getHeight() const = 0;
        virtual bool isFocusable() const = 0;
        virtual void update(const sf::RenderWindow& window){}
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
        virtual void onFocus(){}
        virtual void lostFocus(){}
        virtual void onClick(){}
};

}


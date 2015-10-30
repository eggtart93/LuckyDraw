#pragma once

#include "State.h"
//#include "DrawableEntity.h"
#include "EntityNode.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>

class NotificationDialog : public EntityNode
{
    public:
    NotificationDialog(const std::string& info, const fw::State::Context& context);
    virtual ~NotificationDialog();

    virtual void update(sf::Time dt);
    virtual void onDraw(sf::RenderTarget& target, sf::RenderStates state) const;

    void release();
    void reset();

    void setDisplayText(const sf::String& info);
    void setDisplayTime(float duration);
    void setDisplayPosition(sf::Vector2f pos);
    void setDisplayPosition(float x, float y);
    bool isFinished() const;

    private:
    sf::RectangleShape mFrame;
    sf::Text mDisplayText;
    float mDisplayTime;
    sf::Vector2f mDisplayPos;
    sf::Vector2f mBeginPos;
    bool mIsFinished;
};


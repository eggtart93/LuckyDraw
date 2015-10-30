#include "..\include\SimpleButton.h"

#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#define BUTTON_TEXT_SIZE 12
#define BUTTON_WIDTH 135
#define BUTTON_HEIGHT 34
#define OUT_LINE_THICKNESS 2.f

#define TRANS_EFFECT_LIGHT 0.90f
#define RELEASED_TRANS_EFFECT 0.75f
#define FOCUSED_TRANS_EFFECT 1.f
#define PRESSED_TRANS_EFFECT 0.65f
#define LOCKED_TRANS_EFFECT 0.01f

#define COLOR_NOT_FOCUSABLE sf::Color(189, 195, 199, 200)
#define COLOR_ON_RELEASED sf::Color(0, 0, 0, 255)

using namespace GUI;

SimpleButton::SimpleButton(size_t radius)
:   mCallback(),
    mShape(radius,3),
    mColor(COLOR_ON_RELEASED),
    mScale(1.f, 1.f),
    mFocusable(true),
    mState(State::ON_RELEASED)
{
    Graphics::centerOrigin(mShape);
    mShape.setFillColor(mColor);
}


SimpleButton::~SimpleButton()
{
}

uint16_t SimpleButton::getWidth() const
{
    return (uint16_t)mShape.getGlobalBounds().width;
}

uint16_t SimpleButton::getHeight() const
{
    return (uint16_t)mShape.getGlobalBounds().height;
}

bool SimpleButton::isFocusable() const
{
    return true;
}

void SimpleButton::update(const sf::RenderWindow& window)
{
    if (mFocusable)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        bool isMouseOn = mousePos.x >= (getGlobalPosition().x - getWidth() * .5f)
            && mousePos.x <= (getGlobalPosition().x + getWidth() * .5f)
            && mousePos.y >= (getGlobalPosition().y - getHeight() * .5f)
            && mousePos.y <= (getGlobalPosition().y + getHeight() * .5f);

        if (isMouseOn)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                onPress();
            }
            else
            {
                if (mState == SimpleButton::ON_PRESSED) { onClick(); }
                onFocus();
            }
        }
        else
        {
            lostFocus();
        }
    }

    updateAppearance();
}

void SimpleButton::setButtonColor(sf::Color color)
{
    mColor = color;
}

void SimpleButton::setButtonSize(float w, float h)
{
    mScale.x = w / mShape.getGlobalBounds().width;
    mScale.y = h / mShape.getGlobalBounds().height;
    mShape.setScale(mScale);
}

void SimpleButton::setRotation(float deg)
{
    mShape.setRotation(deg);
}

void SimpleButton::setCallback(Callback callback)
{
    mCallback = std::move(callback);
}

void SimpleButton::setFocusable(bool flag)
{
    mFocusable = flag;
}

void SimpleButton::onFocus()
{
    mState = SimpleButton::ON_FOCUSED;
}

void SimpleButton::lostFocus()
{
    mState = SimpleButton::ON_RELEASED;
}

void SimpleButton::onPress()
{
    mState = SimpleButton::ON_PRESSED;
}

void SimpleButton::onClick()
{
    if (mCallback) { mCallback(); }
}

void SimpleButton::onLock()
{
    mState = SimpleButton::ON_LOCKED;
}

void SimpleButton::onDraw(sf::RenderTarget& target, sf::RenderStates state) const 
{
    target.draw(mShape, state);
}

void SimpleButton::updateAppearance() 
{
    sf::Color color = mColor;
    if (!mFocusable)
    {
        mShape.setFillColor(COLOR_NOT_FOCUSABLE);
        mShape.setScale(mScale);
        return;
    }

    switch (mState)
    {
    case ON_LOCKED:
        break;

    case ON_RELEASED:
        color.a = 255 * RELEASED_TRANS_EFFECT;
        mShape.setFillColor(color);
        mShape.setScale(mScale);
        break;

    case ON_FOCUSED:
        color.a = 255 * FOCUSED_TRANS_EFFECT;
        mShape.setFillColor(color);
        mShape.setScale(mScale.x + .1f, mScale.y + .1f);
        break;

    case ON_PRESSED:
        color.a = 255 * PRESSED_TRANS_EFFECT;
        mShape.setFillColor(color);
        mShape.setScale(mScale.x - .1f, mScale.y - .1f);
        break;

    default:
        break;
    }
}
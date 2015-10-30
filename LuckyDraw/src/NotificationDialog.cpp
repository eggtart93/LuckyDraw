#include "NotificationDialog.h"

#include "ResourceHolder.hpp"
#include "ResourceTypes.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#define DIALOG_WIDTH 300.f
#define DIALOG_HEIGHT 50.f
#define DIALOG_TEXT_SIZE 30
#define DIALOG_TEXT_COLOR sf::Color(255, 255, 255, 255)
#define DIALOG_DISPLAY_TIME 3.f

#define DIALOG_BG_COLOR sf::Color(26, 188, 156, 160)
//#define DIALOG_BG_COLOR sf::Color(0, 0, 0, 150)

#define MOVEMENT_SPEED 250.f

NotificationDialog::NotificationDialog(const std::string& info,
                                       const fw::State::Context& context)
: mFrame(sf::Vector2f(DIALOG_WIDTH, DIALOG_HEIGHT)),
  mDisplayText(info, context.fonts->get(Fonts::Chinese), DIALOG_TEXT_SIZE),
  mDisplayTime(DIALOG_DISPLAY_TIME),
  mIsFinished(true)
{
    mFrame.setFillColor(DIALOG_BG_COLOR);
    Graphics::centerOrigin(mFrame);

    mDisplayText.setColor(DIALOG_TEXT_COLOR);
    Graphics::centerOrigin(mDisplayText);

    mDisplayPos.x = context.window->getView().getCenter().x;
    mDisplayPos.y = mFrame.getGlobalBounds().height * .5f;
    mBeginPos.x = mDisplayPos.x;
    mBeginPos.y = -mFrame.getGlobalBounds().height;
    setPosition(mBeginPos);
}

NotificationDialog::~NotificationDialog() {}

void NotificationDialog::update(sf::Time dt)
{
    if (mDisplayTime > 0.f)
    {
        if(getPosition() != mDisplayPos)
        {
            sf::Vector2f s = mDisplayPos - getPosition();
            sf::Vector2f dir = unitVector(s);
            setVelocity(dir * MOVEMENT_SPEED);
            if (magnitudeOf(getVelocity() * dt.asSeconds()) > magnitudeOf(s))
            {
                setVelocity(0.f, 0.f);
                setPosition(mDisplayPos);
            }
        }
        else
        {
            mDisplayTime -= dt.asSeconds();
        }
    }
    else
    {
        if(getPosition().y > -mFrame.getGlobalBounds().height * .5f)
        {
            setVelocity(0.f, -MOVEMENT_SPEED);
        }
        else
        {
            setVelocity(0.f, 0.f);
            mIsFinished = true;
        }
    }
    EntityNode::update(dt);
}

void NotificationDialog::onDraw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(mFrame, state);
    target.draw(mDisplayText, state);
}

void NotificationDialog::release()
{
    mIsFinished = false;
}

void NotificationDialog::reset()
{
    setDisplayTime(DIALOG_DISPLAY_TIME);
    setDisplayPosition(mBeginPos.x, mFrame.getGlobalBounds().height * .5f);
    setPosition(mBeginPos);
    mIsFinished = true;
}

void NotificationDialog::setDisplayText(const sf::String& info)
{
    mDisplayText.setString(info);
    Graphics::centerOrigin(mDisplayText);
}

void NotificationDialog::setDisplayTime(float duration)
{
    mDisplayTime = duration;
}

void NotificationDialog::setDisplayPosition(sf::Vector2f pos)
{
    mDisplayPos = pos;
}

void NotificationDialog::setDisplayPosition(float x, float y)
{
    mDisplayPos.x = x;
    mDisplayPos.y = y;
}

bool NotificationDialog::isFinished() const
{
    return mIsFinished;
}


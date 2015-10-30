#include "BottomBar.h"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>

#define BAR_WIDTH 600.f
#define BAR_HEIGHT 60.f
#define BAR_BG_COLOR sf::Color(13, 15, 20, 245)
//#define BAR_BG_COLOR sf::Color(52, 73, 94, 240)
//#define BAR_BG_COLOR sf::Color(34, 49, 63, 240)
#define BAR_SLIDING_SPEED 250.f
#define BUTTON_GAP 20.f

BottomBar::BottomBar(const sf::RenderWindow& win)
: mWin(win),
  mFrame(sf::Vector2f(win.getView().getSize().x, BAR_HEIGHT)),
  mButtons(),
  mAnimator(*this),
  mIsBarHidden(true)
{
    mFrame.setFillColor(BAR_BG_COLOR);
    Graphics::centerOrigin(mFrame);

    sf::Vector2f beginPos, displayPos;
    displayPos.x = .5f * win.getView().getSize().x;
    displayPos.y = win.getView().getSize().y - .5f * mFrame.getGlobalBounds().height;
    beginPos.x = displayPos.x;
    beginPos.y = win.getView().getSize().y + .5f * mFrame.getGlobalBounds().height;

    mAnimator.setAnimation(displayPos);

    setPosition(beginPos);
}

BottomBar::~BottomBar(){}

void BottomBar::update(sf::Time dt)
{
    if (isOnFocus())
    {
        if (mIsBarHidden)
        {
            sf::Vector2f dest;
            dest.x = .5f * mWin.getView().getSize().x;
            dest.y = mWin.getView().getSize().y - .5f * mFrame.getGlobalBounds().height;
            mAnimator.setAnimation(dest);
            mAnimator.activate(true);
            mIsBarHidden = false;
        }

        for (auto it = mButtons.begin(); it != mButtons.end(); ++it)
        {
            (*it)->update(mWin);
        }

    }
    else
    {
        if (!mIsBarHidden)
        {
            sf::Vector2f dest;
            dest.x = .5f * mWin.getView().getSize().x;
            dest.y = mWin.getView().getSize().y + .5f * mFrame.getGlobalBounds().height;
            mAnimator.setAnimation(dest);
            mAnimator.activate(true);
            mIsBarHidden = true;
        }
    }

    mAnimator.update(dt);
    /* NOTE: when using Animiation, don't invoke EntityNode::update
             manually, let the animator do its job
       EntityNode::update(dt);
    */
    
}

void BottomBar::onDraw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(mFrame, state);
}

bool BottomBar::onMove(sf::Time dt, sf::Vector2f dest)
{
    if (getPosition() == dest) { return false; }
    else
    {
        sf::Vector2f s = dest - getPosition();
        sf::Vector2f dir = unitVector(s);
        setVelocity(dir * BAR_SLIDING_SPEED);
        if (magnitudeOf(getVelocity() * dt.asSeconds()) > magnitudeOf(s))
        {
            setVelocity(0.f, 0.f);
            setPosition(dest);
            return false;
        }
        return true;
    }
}

bool BottomBar::isOnFocus()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(mWin);
    bool isMouseOn = false;
    if (mIsBarHidden)
    {
        isMouseOn = mousePos.x >= (getPosition().x - .5f * mFrame.getGlobalBounds().width)
            && mousePos.x <= (getPosition().x + .5f * mFrame.getGlobalBounds().width)
            && mousePos.y >= (mWin.getView().getSize().y - 20.f)
            && mousePos.y <= (mWin.getView().getSize().y);
    }
    else
    {
        isMouseOn = mousePos.x >= (getPosition().x - .5f * mFrame.getGlobalBounds().width)
            && mousePos.x <= (getPosition().x + .5f * mFrame.getGlobalBounds().width)
            && mousePos.y >= (mWin.getView().getSize().y - mFrame.getGlobalBounds().height)
            && mousePos.y <= (mWin.getView().getSize().y);
    }
    return isMouseOn;
}

void BottomBar::addButton(GUI::Button::Ptr p_btn)
{
    mButtons.push_back(p_btn.get());
    attachChild(std::move(p_btn));
}

void BottomBar::pack()
{
    float max_w = mFrame.getGlobalBounds().width;

    if (mButtons.size() == 1)
    {
        mButtons.front()->setPosition(getPosition());
    }
    else if (mButtons.size() > 1)
    {
        //float max_w = mFrame.getGlobalBounds().width;
        unsigned int n = mButtons.size();
        float w = mButtons.front()->getWidth();
        float tot_w = n * w + (n - 1) * BUTTON_GAP;
        float x = .5f * (max_w - tot_w) + .5f * w;
        x = x - getPosition().x; // minus the transform of the Bar itself
        for (size_t i = 0; i < n; ++i)
        {
            mButtons[i]->setPosition(x + i * (BUTTON_GAP + w), mFrame.getPosition().y);
        }
    }
}


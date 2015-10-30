#pragma once

#include "State.h"
//#include "DrawableEntity.h"
#include "EntityNode.h"
#include "Button.h"
#include "MoveAnimation.h"

class BottomBar : public EntityNode
{
    public:
    typedef std::unique_ptr<BottomBar> Ptr;

    public:
    BottomBar(const sf::RenderWindow& win);
    virtual ~BottomBar();

    virtual void update(sf::Time dt);
    virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;

    void addButton(GUI::Button::Ptr p_btn);
    void pack();

    private:
    bool onMove(sf::Time dt, sf::Vector2f dest);
    bool isOnFocus();

    private:
    const sf::RenderWindow& mWin;
    sf::RectangleShape mFrame;
    std::vector<GUI::Button*> mButtons;
    MoveAnimation mAnimator;
    
    bool mIsBarHidden;
};


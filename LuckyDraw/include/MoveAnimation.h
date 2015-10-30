#pragma once

#include "EntityNode.h"

#include <deque>

#define DEFAULT_SPEED 250.f
#define DEFAULT_DURATION 1.f 

class MoveAnimation
{
    public:
    MoveAnimation();
    MoveAnimation(EntityNode& node);
    virtual ~MoveAnimation();

    virtual void update(sf::Time dt);

    virtual void setAnimateObj(EntityNode& node);
    virtual void setAnimation(sf::Vector2f destP, float speed = DEFAULT_SPEED);
    virtual void setAnimation(const std::deque<sf::Vector2f>& path, 
                                    float speed = DEFAULT_SPEED,
                                    float interval = DEFAULT_DURATION);

    virtual void activate(bool flag);
    bool isActive() const;

    private:
    bool onMove(sf::Time dt);

    private:
    EntityNode* p_mNode;
    std::deque<sf::Vector2f> mCoordinates;
    float mSpeed;
    float mDuration;
    float mTimeBuf;
    bool mIsActive;
};


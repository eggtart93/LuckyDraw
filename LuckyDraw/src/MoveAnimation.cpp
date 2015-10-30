#include "MoveAnimation.h"
#include "Utility.hpp"

MoveAnimation::MoveAnimation()
: p_mNode(nullptr),
  mCoordinates(),
  mSpeed(0.f),
  mDuration(0.f),
  mTimeBuf(0.f),
  mIsActive(false)
{}

MoveAnimation::MoveAnimation(EntityNode& node)
: p_mNode(&node),
  mCoordinates(),
  mSpeed(0.f),
  mDuration(0.f),
  mTimeBuf(0.f),
  mIsActive(false)
{}

MoveAnimation::~MoveAnimation() {}

void MoveAnimation::update(sf::Time dt)
{
    if (mIsActive)
    {
        mIsActive = onMove(dt);
    }
}

void MoveAnimation::setAnimateObj(EntityNode& node)
{
    p_mNode = &node;
}

void MoveAnimation::setAnimation(sf::Vector2f destP, float speed)
{
    mCoordinates.clear();
    mCoordinates.push_back(destP);
    mSpeed = speed;
}

void MoveAnimation::setAnimation(const std::deque<sf::Vector2f>& path,
                                    float speed, float interval)
{
    mCoordinates = path;
    mSpeed = speed;
    mDuration = interval;
}

void MoveAnimation::activate(bool flag)
{
    mIsActive = flag;
}

bool MoveAnimation::isActive() const
{
    return mIsActive;
}

bool MoveAnimation::onMove(sf::Time dt)
{
    if (p_mNode == nullptr || mCoordinates.empty()) { return false; }

    if (p_mNode->getPosition() == mCoordinates.front())
    {
        if (mCoordinates.size() > 1)
        {
            if (mTimeBuf >= mDuration)
            {
                mTimeBuf = 0.f;
                mCoordinates.pop_front();
                return false;
            }
            mTimeBuf -= dt.asSeconds();
        }
        else { return false; }
    }
    else
    {
        sf::Vector2f s = mCoordinates.front() - p_mNode->getPosition();
        sf::Vector2f dir = unitVector(s);
        p_mNode->setVelocity(dir * mSpeed);
        if (magnitudeOf(p_mNode->getVelocity() * dt.asSeconds()) > magnitudeOf(s))
        {
            p_mNode->setVelocity(0.f, 0.f);
            p_mNode->setPosition(mCoordinates.front());
            if (mCoordinates.size() < 2) { mCoordinates.pop_front(); }
            return false;
        }
        p_mNode->move(p_mNode->getVelocity() * dt.asSeconds());
    }
    return true;
}


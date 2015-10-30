#include "EntityNode.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

EntityNode::EntityNode()
    : p_mDrawableObj(nullptr),
      mVelocity(0.f, 0.f)
{}

void EntityNode::update(sf::Time dt)
{
    move(mVelocity * dt.asSeconds());
}

void EntityNode::onDraw(sf::RenderTarget& target, sf::RenderStates state) const
{
    // apply transform to the drawable object
    //state.transform *= getTransform();
    if (p_mDrawableObj != nullptr) { target.draw(*p_mDrawableObj, state); }
}

void EntityNode::setDrawableObj(const sf::Drawable& drawable)
{
    p_mDrawableObj = &drawable;
}

void EntityNode::setVelocity(float vx, float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

void EntityNode::setVelocity(sf::Vector2f v)
{
    mVelocity = v;
}

void EntityNode::accelerate(float ax, float ay)
{
    mVelocity.x += ax;
    mVelocity.y += ay;
}

void EntityNode::accelerate(sf::Vector2f a)
{
    mVelocity += a;
}

const sf::Vector2f& EntityNode::getVelocity() const
{
    return mVelocity;
}

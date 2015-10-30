#include "DrawableEntity.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

DrawableEntity::DrawableEntity()
    : p_mDrawableObj(nullptr),
      mVelocity(0.f, 0.f)
{}

void DrawableEntity::update(sf::Time dt)
{
    move(mVelocity * dt.asSeconds());
}

void DrawableEntity::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    // apply transform to the drawable object
    state.transform *= getTransform();
    if (p_mDrawableObj != nullptr) { target.draw(*p_mDrawableObj, state); }
}

void DrawableEntity::setDrawableObj(const sf::Drawable& drawable)
{
    p_mDrawableObj = &drawable;
}

void DrawableEntity::setVelocity(float vx, float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

void DrawableEntity::setVelocity(sf::Vector2f v)
{
    mVelocity = v;
}

void DrawableEntity::accelerate(float ax, float ay)
{
    mVelocity.x += ax;
    mVelocity.y += ay;
}

void DrawableEntity::accelerate(sf::Vector2f a)
{
    mVelocity += a;
}

const sf::Vector2f& DrawableEntity::getVelocity() const
{
    return mVelocity;
}

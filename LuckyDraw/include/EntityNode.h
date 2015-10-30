#pragma once

#include "SceneNode.h"

#include <SFML/System/Time.hpp>

class EntityNode : public SceneNode
{
    public:
        EntityNode();
        EntityNode(sf::Drawable* drawable);
        virtual void update(sf::Time dt);
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates state) const;

        void setDrawableObj(const sf::Drawable& drawable);

        void setVelocity(float vx, float vy);
        void setVelocity(sf::Vector2f v);
        void accelerate(float ax, float ay);
        void accelerate(sf::Vector2f a);

        const sf::Vector2f& getVelocity() const;

    private:
        const sf::Drawable* p_mDrawableObj;
        sf::Vector2f mVelocity;
};


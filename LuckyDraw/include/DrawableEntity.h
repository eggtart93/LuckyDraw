#ifndef DRAWABLEENTITY_H
#define DRAWABLEENTITY_H

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

class DrawableEntity : public sf::Drawable, public sf::Transformable, public sf::NonCopyable
{
    public:
        DrawableEntity();
        DrawableEntity(sf::Drawable* drawable);
        virtual void update(sf::Time dt);
        virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const;

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

#endif // DRAWABLEENTITY_H

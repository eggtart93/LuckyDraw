#pragma once

#include "Component.h"
#include "ResourceHolder.hpp"
#include "ResourceTypes.hpp"

#include <SFML/Graphics/CircleShape.hpp>

#include <functional>

namespace GUI
{
    class SimpleButton : public Component
    {
        public:
        typedef std::function<void()> Callback;
        typedef std::unique_ptr<SimpleButton> Ptr;

        enum State
        {
            ON_LOCKED,
            ON_RELEASED,
            ON_FOCUSED,
            ON_PRESSED,
        };

        public:
        SimpleButton(size_t radius);
        ~SimpleButton();

        virtual uint16_t getWidth() const;
        virtual uint16_t getHeight() const;
        virtual bool isFocusable() const;

        virtual void update(const sf::RenderWindow& window);

        void setCallback(Callback callback);
        
        void setButtonColor(sf::Color color);
        void setButtonSize(float width, float height);
        void setRotation(float deg);
        void setFocusable(bool flag);

        protected:
        virtual void onFocus();
        virtual void lostFocus();
        virtual void onPress();
        virtual void onClick();
        virtual void onLock();

        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        void updateAppearance();

        protected:
        Callback mCallback;
        sf::CircleShape mShape;
        sf::Color mColor;
        sf::Vector2f mScale;
        bool mFocusable;
        State mState;
    };
}
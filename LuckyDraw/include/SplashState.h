#pragma once

#include "State.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Utf.hpp>

class SplashState : public fw::State
{
    public:
        SplashState(StateStack& sStack, State::Context context);
        virtual bool processEvent(const sf::Event& e);
        virtual bool update(sf::Time dt);
        virtual void draw();

    private:
        sf::Text mEndingText;
        float mDuration;
};


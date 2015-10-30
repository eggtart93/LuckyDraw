#include "SplashState.h"
#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "ResourceTypes.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

using namespace fw;

#define SPLASH_SCREEN_W 400
#define SPLASH_SCREEN_H 180
#define DURATION 5.f
SplashState::SplashState(StateStack& sStack, State::Context context)
: State(sStack, context)
, mEndingText()
{
    context.window->create(sf::VideoMode(SPLASH_SCREEN_W, SPLASH_SCREEN_H), "App", sf::Style::None);
    context.window->setIcon(256, 256, context.icon->getPixelsPtr());
    context.window->setVerticalSyncEnabled(true);
    context.window->setKeyRepeatEnabled(false);

    mEndingText.setFont(context.fonts->get(Fonts::System));
    mEndingText.setCharacterSize(24);
    mEndingText.setColor(sf::Color(236, 240, 241, 255));
    mEndingText.setString(L"Copyright (c) 2015 Jin Kuan Zhou\n<zhoujk@mcmaster.ca>");
    Graphics::centerOrigin(mEndingText);
    mEndingText.setPosition(context.window->getSize().x / 2.f,
                             context.window->getSize().y / 2.f - 30.f);
}

bool SplashState::processEvent(const sf::Event& e)
{
    if(  e.type == sf::Event::KeyReleased
       || e.type == sf::Event::MouseButtonReleased)
    {
        requestStackPop();
        requestStackPush(States::GAME);
    }
    return true;
}

bool SplashState::update(sf::Time dt)
{
    if (mDuration > DURATION)
    {
        requestStackPop();
        requestStackPush(States::GAME);
    }

    sf::String str = L"Loading ";
    str += std::to_string((int)(100 * mDuration / DURATION));
    str += L" %\n\nCopyright (c) 2015 Jin Kuan Zhou\n<zhoujk@mcmaster.ca>";
    
    mEndingText.setString(str);
    mDuration += dt.asSeconds();
    return true;
}

void SplashState::draw()
{
    //getContext().window->clear(sf::Color(0, 20, 46, 255));
    getContext().window->clear(sf::Color(13, 15, 20, 255));
    getContext().window->draw(mEndingText);
}

#pragma once

#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "ResourceTypes.hpp"
//#include "SoundPlayer.h"
#include "StateStack.h"
#include "Settings.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

class Application
{
    public:
        Application();
        void run();

    private:
        void processInput();
        void update(sf::Time dt);
        void render();
        void registerStates();

#ifdef Debug
        void updateRunTimeInfo(sf::Time dt);
#endif

    private:
        static const sf::Time TimePerFrame;
        sf::RenderWindow mWindow;
        sf::Image mIcon;
        bool mIsOnFocused;
        TextureHolder mTexturesPool;
        FontHolder mFontsPool;
        //SoundPlayer mSoundPlayer;
        StateStack mStateStack;

#ifdef Debug
        sf::Time mUpdateTime;
        sf::Text mRunTimeInfo;
        size_t mNumOfFrames;
        size_t mAvgFPS;
        size_t mTicks;
#endif
};


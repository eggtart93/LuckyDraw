#include "Application.h"

#include "SplashState.h"
#include "GameState.h"


#include "TraceLog.hpp"


#define RUNTIME_INFO_POS_X 5.f
#define RUNTIME_INFO_POS_Y 5.f

#define DisplayRuntimeInfo true;

using namespace GUI;

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mWindow(),
 mIsOnFocused(true),
 mTexturesPool(),
 mFontsPool(),
 //mSoundPlayer(),
 mStateStack(State::Context(mWindow, mTexturesPool, mFontsPool, mIcon))
#ifdef Debug
 ,mUpdateTime(sf::Time::Zero),
 mRunTimeInfo(),
 mNumOfFrames(0),
 mAvgFPS(0),
 mTicks(0)
#endif
{
    // load resources
    mTexturesPool.load(Textures::RollingWinBackground, "resources/images/bg.jpg");
    mTexturesPool.load(Textures::Particle, "resources/textures/Particle.png");
    mTexturesPool.load(Textures::Gold, "resources/textures/gold.png");
    mTexturesPool.load(Textures::Star, "resources/textures/star_sl.png");
    //mTexturesPool.load(Textures::Banana, "resources/textures/banana_2l.png");
    //mTexturesPool.load(Textures::Coin, "resources/textures/coin_m.png");
    mFontsPool.load(Fonts::System, "resources/fonts/Sansation.ttf");
    mFontsPool.load(Fonts::Digit, "resources/fonts/QuartzMS.TTF");
    mFontsPool.load(Fonts::Chinese, "resources/fonts/wawa5.ttc");
    //mSoundPlayer.load();
    mIcon.loadFromFile("resources/images/icon.png");

    mWindow.create(sf::VideoMode(1, 1), "App", sf::Style::None);
    mWindow.setIcon(256, 256, mIcon.getPixelsPtr());
    mWindow.setVerticalSyncEnabled(true);
    mWindow.setKeyRepeatEnabled(false);

#ifdef Debug
    mRunTimeInfo.setFont(mFontsPool.get(Fonts::System));
    mRunTimeInfo.setCharacterSize(SYSTEM_FONT_SIZE);
    mRunTimeInfo.setColor(sf::Color(255,70,70,255));
    mRunTimeInfo.setPosition(RUNTIME_INFO_POS_X,RUNTIME_INFO_POS_Y);
#endif

    registerStates();
    mStateStack.push(States::SPLASH);
}

void Application::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen())
    {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while(timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;
            processInput();
            update(TimePerFrame);

            if(mStateStack.isEmpty())
            {
                mWindow.close();
                return;
            }
        }

#ifdef Debug
        updateRunTimeInfo(dt);
#endif
        render();
    }
}

void Application::processInput()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        mStateStack.processEvent(event);
    }
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void Application::render()
{
    mWindow.clear(sf::Color(236, 240, 241, 255));
    mStateStack.draw();

    mWindow.setView(mWindow.getDefaultView());

#ifdef Debug
    mWindow.draw(mRunTimeInfo);
#endif

    mWindow.display();
}

#ifdef Debug
void Application::updateRunTimeInfo(sf::Time dt)
{
    mUpdateTime += dt;
    mNumOfFrames++;

    if(mUpdateTime >= sf::seconds(1.0f))
    {
        mAvgFPS = (mAvgFPS * mTicks + mNumOfFrames) / (mTicks + 1);
        mTicks++;
        mRunTimeInfo.setString(
            "Frames / Second = "
            + toString(mNumOfFrames) + "\n"
            + "Time / Update = "
            + toString(mUpdateTime.asMicroseconds() / mNumOfFrames)
            + "us\n"
            + "Avg. FPS = "
            + toString(mAvgFPS)
            + "\nMouse Position = "
            + toString(sf::Mouse::getPosition(mWindow).x)
            + ","
            + toString(sf::Mouse::getPosition(mWindow).y));

        mUpdateTime -= sf::seconds(1.0f);
        mNumOfFrames = 0;
    }
}
#endif

void Application::registerStates()
{
    mStateStack.registerState<GameState>(States::GAME);
    mStateStack.registerState<SplashState>(States::SPLASH);
}

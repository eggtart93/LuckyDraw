#pragma once

#include "ResourceHolder.hpp"
#include "ResourceTypes.hpp"
#include "Button.h"
#include "BottomBar.h"
#include "RollingWindow.h"
#include "State.h"
#include "LotteryDraw.h"
#include "HistoryDialog.h"
#include "FileIO.h"

#include <deque>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf
{
    // Forward declaration of SFML classes
    class RenderWindow;
    class Text;
    class Time;
}

class GameScreen : public sf::NonCopyable
{
    public:
    enum EventCode
    {
        NONE = 0,
        INIT_COMPLETED,
        ITEM_PICKED,
        ITEM_CANCELED,
        LIST_EMPTY,
        WRITE_LOG_FAILURE,
        READ_DATA_FAILURE,
        RESTART_COMPLETED,
        CHANGE_ANIMATION,
        //CHANGE_TO_SLIDE,
        //CHANGE_TO_REFRESH,
        BLOOM_ENABLE,
        BLOOM_DISABLE,
        SWITCH_TO_FULL_SCREEN,
        SWITCH_TO_DEFAULT_WINDOW,
        REQUEST_EXIT
    };

    public:
    explicit GameScreen(const fw::State::Context& context);
    bool update(sf::Time dt);
    void draw();
    int pollEvent();

    private:
    void loadTextures();
    void initScreen();

    private:
    fw::State::Context mContext;
    FileIO mFileHandler;
    RollingWindow mRollingWindow;

    LotteryDraw mLotterySys;
    bool mIsLotSysRunning;

    SceneNode mSceneGraph;
    BottomBar* p_mBar;
    GUI::Button* p_mStartBtn;
    GUI::Button* p_mFullScreen;
    HistoryDialog* p_mHistDialog;

    std::deque<int> mEventQueue;
    bool mIsFullScreen;
    bool mIsRunning;
};


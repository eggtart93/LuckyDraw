#include "GameScreen.h"
#include "Utility.hpp"
#include "Settings.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include <vector>

//#define TRACE_ENABLE
#include "TraceLog.hpp"

#define DATABASE_PATH "dat/db.dat"
#define LOG_PATH "dat/sys.log"

using namespace GUI;

GameScreen::GameScreen(const fw::State::Context& context)
: mContext(context),
  mFileHandler(DATABASE_PATH, LOG_PATH),
  mRollingWindow(context),
  mIsLotSysRunning(false),
  p_mBar(nullptr),
  p_mStartBtn(nullptr),
  p_mHistDialog(nullptr),
  mEventQueue(),
  mIsFullScreen(false),
  mIsRunning(true)
{
    //loadTextures();
    initScreen();
    std::vector<std::string> data_list(std::move(mFileHandler.readData()));
    if (data_list.empty())
    {
        mEventQueue.push_back(READ_DATA_FAILURE);
    }
    else
    {
        mLotterySys.load(data_list);
        mLotterySys.reset(false);
        mRollingWindow.requestOn();
        mEventQueue.push_back(INIT_COMPLETED);
    }
    mFileHandler.writeLog("Initialization completed");
}

bool GameScreen::update(sf::Time dt)
{
    if (mIsRunning)
    {
        /* update sliding bottom bar */
        p_mBar->update(dt);

        /* update history dialog */
        p_mHistDialog->update(dt);

        /* update start button */
        p_mStartBtn->update(*(mContext.window));

        /* update lottery window */
        if (mRollingWindow.hasShowedContent())
        {
            if (mRollingWindow.getCurrentMode() == RollingWindow::SLIDE_MODE)
            {
                mRollingWindow.setNextStr(mLotterySys.getNext()->data);
            }
            else
            {
                mRollingWindow.setNextStr(mLotterySys.getNextRand()->data);
            }
        }

        mRollingWindow.update(dt);
        return true;
    }
    return false;
}

void GameScreen::draw()
{
    /* reset the window */
    mContext.window->setView(mContext.window->getDefaultView());

    /* draw all elements on the window */
    mRollingWindow.draw();
    mContext.window->draw(mSceneGraph);
}

int GameScreen::pollEvent()
{
    if (!mEventQueue.empty())
    {
        int event = mEventQueue.front();
        mEventQueue.pop_front();
        return event;
    }
    return EventCode::NONE;
}

void GameScreen::initScreen()
{
    float winWidth = (float)mContext.window->getSize().x;

    /* add rolling window */
    mRollingWindow.setPosition(0.5f * winWidth, .5f * mRollingWindow.getSize().y);


    /* add start button */
    Button::Ptr startBtn(new Button(Button::Text,
                                        *(mContext.fonts),
                                        *(mContext.textures)));
    p_mStartBtn = startBtn.get();
    p_mStartBtn->setText(sf::String(L"开始"));
    p_mStartBtn->setButtonColor(sf::Color(54, 215, 183, 255));
    p_mStartBtn->setButtonSize(GSCR_BUTTON_WIDTH, GSCR_BUTTON_HEIGHT);
    p_mStartBtn->setTextSize(GSCR_BUTTON_TEXT_SIZE);
    //p_mStartBtn->setButtonColor(sf::Color(175, 255, 217, 255));
    p_mStartBtn->setCallback([this]()
    {
        if (mIsLotSysRunning)
        {
            if (mRollingWindow.requestStop())
            {
                std::string str = mLotterySys.pick()->data;
                mRollingWindow.setResultStr(str);
                mFileHandler.writeLog("picked " + str);
                p_mStartBtn->setText(sf::String(L"开始"));
                mIsLotSysRunning = false;
                mEventQueue.push_back(ITEM_PICKED);
            }
        }
        else
        {
            if (!mLotterySys.isAllPickedOut())
            {
                if (mRollingWindow.requestRun())
                {
                    p_mStartBtn->setText(sf::String(L"停止"));
                    mIsLotSysRunning = true;
                }
            }
            else
            {
                mEventQueue.push_back(LIST_EMPTY);
                TRACE("GameScreen.cpp", "All picked out");
            }
        }
    });
    p_mStartBtn->setPosition(.5f * winWidth, mRollingWindow.getSize().y + 60.f);


    /* create history dialog */
    HistoryDialog::Ptr dialog(new HistoryDialog(sf::String(""), mContext));
    p_mHistDialog = dialog.get();

    //p_mHistDialog->setPosition(.5f * winWidth, mRollingWindow.getSize().y + 60.f);


    /* create restart button */
    Button::Ptr p_restartBtn(new Button(Button::Text,
                                       *(mContext.fonts),
                                       *(mContext.textures)));
    p_restartBtn->setText(sf::String(L"重启"));
    p_restartBtn->setButtonColor(sf::Color(52, 152, 219, 255));
    p_restartBtn->setTextColor(sf::Color(236, 240, 241, 255));
    p_restartBtn->setCallback([this]()
    {
        if (mRollingWindow.requestRestart(INIT_MESSAGE))
        {
            mLotterySys.reset(false);
            mEventQueue.push_back(RESTART_COMPLETED);
            mFileHandler.writeLog("Lottery System restarted");
        }
    });

    /* create switch button */
    Button::Ptr p_switchBtn(new Button(Button::Text,
                                       *(mContext.fonts),
                                       *(mContext.textures)));
    p_switchBtn->setText(sf::String(L"切换动画"));
    p_switchBtn->setButtonColor(sf::Color(46, 204, 113, 255));
    p_switchBtn->setTextColor(sf::Color(236, 240, 241, 255));
    p_switchBtn->setCallback([this]()
    {
        mRollingWindow.switchClbAnim();
        mEventQueue.push_back(CHANGE_ANIMATION);
        mFileHandler.writeLog("Animation changed");
        /*
        if (mRollingWindow.getCurrentMode() == RollingWindow::REFRESH_MODE)
        {
            mRollingWindow.requestChangeMode(RollingWindow::SLIDE_MODE);
            mEventQueue.push_back(CHANGE_TO_SLIDE);
            mFileHandler.writeLog("Changed to slide animation");
        }
        else
        {
            mRollingWindow.requestChangeMode(RollingWindow::REFRESH_MODE);
            mEventQueue.push_back(CHANGE_TO_REFRESH);
            mFileHandler.writeLog("changed to refresh animation");
        }
        */
    });

    /* create cancel button */
    Button::Ptr p_cancelBtn(new Button(Button::Text,
                                        *(mContext.fonts),
                                        *(mContext.textures)));
    p_cancelBtn->setText(sf::String(L"作废"));
    p_cancelBtn->setButtonColor(sf::Color(231, 76, 60, 255));
    p_cancelBtn->setTextColor(sf::Color(236, 240, 241, 255));
    p_cancelBtn->setCallback([this]()
    {
        if (mRollingWindow.requestRestart("0000"))
        {
            mFileHandler.writeLog("Cancled selection " + mLotterySys.getPicked()->data);
            mLotterySys.eraseLastFromHistory();
            mEventQueue.push_back(ITEM_CANCELED);
        }
    });

    /* create Exit button */
    Button::Ptr p_exitBtn(new Button(Button::Text,
                                    *(mContext.fonts),
                                    *(mContext.textures)));
    p_exitBtn->setText(sf::String(L"离开"));
    p_exitBtn->setButtonColor(sf::Color(155, 89, 182, 255));
    p_exitBtn->setTextColor(sf::Color(236, 240, 241, 255));
    p_exitBtn->setCallback([this]()
    {
        if (mRollingWindow.requestOff())
        {
            mEventQueue.push_back(REQUEST_EXIT);
            mIsRunning = false;
            mFileHandler.writeLog("Exiting...");
        }
    });

    /* create history button */
    //sf::Color(241, 196, 15, 255)
    Button::Ptr p_historyBtn(new Button(Button::Text,
                                    *(mContext.fonts),
                                    *(mContext.textures)));
    p_historyBtn->setText(sf::String(L"记录"));
    p_historyBtn->setButtonColor(sf::Color(243, 156, 18, 255));
    p_historyBtn->setTextColor(sf::Color(236, 240, 241, 255));
    p_historyBtn->setCallback([this]()
    {
        if (!mRollingWindow.isRunning())
        {
            if (p_mHistDialog->isDisplay())
            {
                p_mHistDialog->dismiss();
            }
            else
            {
                p_mHistDialog->setContent(mLotterySys.getHistoryInStr());
                p_mHistDialog->display();
            }
        }
    });

    /* create bloom enable button */
    //sf::Color(241, 196, 15, 255)
    Button::Ptr p_bloomBtn(new Button(Button::Text,
                            *(mContext.fonts),
                            *(mContext.textures)));
    p_bloomBtn->setText(sf::String(L"泛光特效"));
    p_bloomBtn->setButtonColor(sf::Color(243, 156, 18, 255));
    p_bloomBtn->setTextColor(sf::Color(236, 240, 241, 255));
    p_bloomBtn->setCallback([this]()
    {
        if (!mRollingWindow.isRunning())
        {
            if (mRollingWindow.isBloomEnable())
            {
                mRollingWindow.setBloomEnable(false);
                mEventQueue.push_back(BLOOM_DISABLE);
            }
            else
            {
                mRollingWindow.setBloomEnable(true);
                mEventQueue.push_back(BLOOM_ENABLE);
            }
        }
    });

    /* create full screen switch button */
    Button::Ptr fullScrBtn(new Button(Button::Text,
                            *(mContext.fonts),
                            *(mContext.textures)));
    p_mFullScreen = fullScrBtn.get();
    p_mFullScreen->setText(sf::String(L"全屏"));
    p_mFullScreen->setButtonColor(sf::Color(155, 89, 182, 255));
    p_mFullScreen->setTextColor(sf::Color(236, 240, 241, 255));
    p_mFullScreen->setCallback([this]()
    {
        if (!mRollingWindow.isRunning())
        {
            if (mIsFullScreen)
            {
                mEventQueue.push_back(SWITCH_TO_DEFAULT_WINDOW);
                p_mFullScreen->setText(sf::String(L"全屏"));
                mFileHandler.writeLog("switch to default window...");
            }
            else
            {
                mEventQueue.push_back(SWITCH_TO_FULL_SCREEN);
                p_mFullScreen->setText(sf::String(L"窗口化"));
                mFileHandler.writeLog("switch to full screen...");
            }
            mIsFullScreen = !mIsFullScreen;
        }
    });

    /* add bottom bar */
    BottomBar::Ptr bar(new BottomBar(*(mContext.window)));
    p_mBar = bar.get();

    /* add buttons to bottom bar */
    p_mBar->addButton(std::move(p_switchBtn));
    p_mBar->addButton(std::move(p_cancelBtn));
    p_mBar->addButton(std::move(p_restartBtn));
    p_mBar->addButton(std::move(p_historyBtn));
    p_mBar->addButton(std::move(p_bloomBtn));
    p_mBar->addButton(std::move(fullScrBtn));
    p_mBar->addButton(std::move(p_exitBtn));
    p_mBar->pack();

    mSceneGraph.attachChild(std::move(bar));
    mSceneGraph.attachChild(std::move(startBtn)); // attach it to the scene graph
    mSceneGraph.attachChild(std::move(dialog));
}


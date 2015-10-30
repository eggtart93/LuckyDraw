#include "GameState.h"

#include "Settings.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/System/String.hpp>

GameState::GameState(StateStack& sStack, fw::State::Context context)
: State(sStack, context),
  p_mGameScreen(nullptr)
{
    resizeWin(DEFAULT);

    p_mGameScreen = std::unique_ptr<GameScreen>(new GameScreen(context));

    std::unique_ptr<NotificationDialog> dialog(new NotificationDialog("", context));
    p_mNotify = dialog.get();
    mNode.attachChild(std::move(dialog));
}

bool GameState::processEvent(const sf::Event& e)
{
    switch (p_mGameScreen->pollEvent())
    {
    case GameScreen::INIT_COMPLETED:
        p_mNotify->setDisplayText(sf::String(L"初始化完成~"));
        p_mNotify->release();
        break;
        
    case GameScreen::ITEM_PICKED:
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"已保存~"));
        p_mNotify->release();
        break;

    case GameScreen::ITEM_CANCELED:
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"删掉了~"));
        p_mNotify->release();
        break;

    case GameScreen::RESTART_COMPLETED:
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"已恢复初始设置"));
        p_mNotify->release();
        break;

    case GameScreen::READ_DATA_FAILURE:
        p_mNotify->setDisplayText(sf::String(L"我找不到文件！"));
        p_mNotify->release();
        break;

    case GameScreen::WRITE_LOG_FAILURE:
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"我没法儿存档！"));
        p_mNotify->release();
        break;

    case GameScreen::LIST_EMPTY:
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"候选列表是空的"));
        p_mNotify->release();
        break;

    case GameScreen::CHANGE_ANIMATION:
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"庆祝动画已切换"));
        p_mNotify->release();
        break;
/*
    case GameScreen::CHANGE_TO_SLIDE:
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"切换至滑动模式"));
        p_mNotify->release();
        break;

    case GameScreen::CHANGE_TO_REFRESH:
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"切换至闪烁模式"));
        p_mNotify->release();
        break;
*/
    case GameScreen::BLOOM_ENABLE:
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"泛光特效开启"));
        p_mNotify->release();
        break;

    case GameScreen::BLOOM_DISABLE:
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"泛光特效关闭"));
        p_mNotify->release();
        break;

    case GameScreen::SWITCH_TO_FULL_SCREEN:
        resizeWin(FULL_SCREEN);
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"全屏模式"));
        p_mNotify->release();
        break;

    case GameScreen::SWITCH_TO_DEFAULT_WINDOW:
        resizeWin(DEFAULT);
        p_mNotify->reset();
        p_mNotify->setDisplayText(sf::String(L"窗口模式"));
        p_mNotify->release();
        break;

    case GameScreen::REQUEST_EXIT:
        requestStackPop();
        break;

    case GameScreen::NONE:
    default:
        break;
    }
    return true;
}

bool GameState::update(sf::Time dt)
{
    if (!p_mNotify->isFinished())
    {
        p_mNotify->update(dt);
    }
    p_mGameScreen->update(dt);
    return true;
}

void GameState::draw()
{
    p_mGameScreen->draw();
    if (!p_mNotify->isFinished())
    {
        this->getContext().window->draw(mNode);
    }
}

void GameState::resizeWin(int mode)
{
    /* This creates a window with antialiasing enable, will decrease fps significantly*/
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    Context context(getContext());

    /* NOTE: use if else for now, will change to switch in the future */
    if (mode == FULL_SCREEN)
    {
        context.window->create(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "App", sf::Style::Fullscreen, settings);
    }
    else
    {
        context.window->create(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "App", sf::Style::None, settings);
    }

    context.window->setIcon(256, 256, context.icon->getPixelsPtr());
    context.window->setVerticalSyncEnabled(true);
    context.window->setKeyRepeatEnabled(false);

}
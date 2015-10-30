#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "State.h"
#include "GameScreen.h"
#include "NotificationDialog.h"
#include "SceneNode.h"

class GameState : public fw::State
{
    public:
        GameState(StateStack& stateStack, Context context);

        virtual bool processEvent(const sf::Event& e);
        virtual bool update(sf::Time dt);
        virtual void draw();

    private:
    enum WinMode
    {
        DEFAULT,
        FULL_SCREEN
    };

    private:
    void resizeWin(int mode);

    private:
        std::unique_ptr<GameScreen> p_mGameScreen;
        SceneNode mNode;
        NotificationDialog* p_mNotify;
        int mMode;
};

#endif // GAMESTATE_H

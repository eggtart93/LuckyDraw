#pragma once

#include "State.h"
#include "DrawableEntity.h"
#include "BloomEffect.hpp"
#include "ParticleSystem.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <functional>
#include <deque>
#include <string>

class RollingWindow : public sf::NonCopyable
{
    public:
    enum AnimationMode
    {
        REFRESH_MODE,
        SLIDE_MODE
    };

    enum State
    {
        OFF,
        ON_INIT,
        ON_READY,
        ON_RUN,
        ON_RUN_LOCKED,
        ON_RESTART
    };

    public:
    explicit RollingWindow(fw::State::Context context);

    void update(sf::Time dt);
    void draw();

    void setSize(unsigned int width, unsigned int height);
    void setPosition(float posX, float posY);
    void setBloomEnable(bool flag);
    void switchClbAnim();

    bool requestOn();
    bool requestRun();
    bool requestRestart(const std::string& str);
    bool requestStop();
    bool requestOff();
    bool requestChangeMode(int mode);

    void setNextStr(const std::string& str);
    void setResultStr(const std::string& str);


    bool isRunning() const;
    bool isBloomEnable() const;
    bool hasShowedContent() const;
    int getCurrentMode() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;

    private:
    enum EntityStatus
    {
        ACCELERATING,
        DECELERATING,
        STOPPED,
        REACHED_MAX_SPEED
    };
    enum CMD
    {
        TURN_ON,
        RUN,
        RESTART,
        STOP,
        TURN_OFF,
        CHANGE_MODE
    };

    private:
    bool onInit(sf::Time dt);
    bool onRun(sf::Time dt);
    bool onRunSlideMode(sf::Time dt);
    bool onRunRefreshMode(sf::Time dt);
    bool onRestart(sf::Time dt);
    void processNextCMD();

    private:
    sf::RenderWindow& mBaseWindow;
    sf::RenderTexture mPaint;
    sf::RenderTexture mOutput;
    sf::FloatRect mRollingBounds;
    sf::Sprite mWindowSprite;
    sf::Sprite mBgSprite;

    BloomEffect mBloomEffect;
    ParticleSystem mBgParticles;
    ParticleSystem mParticleSys;
    bool mBloomEnable;
    bool mIsBloomSupported;

    DrawableEntity mRollingEntity;
    EntityStatus mEntityStatus;
    sf::FloatRect mEntityBounds;
    sf::Text mText;

    std::deque<CMD> mPendingCMDsQueue;
    bool mIsActionCompleted;
    bool mIsRequestStop;
    State mPrevState;
    State mState;
    int mAniMode;

    float mDistBuf;
    float mDistBuf2;
    float mTimeBuf;

    std::string mNextStr;
    std::string mResultStr;
    bool mHasShowedContent;
};


#include "RollingWindow.h"
#include "State.h"
#include "ResourceHolder.hpp"
#include "ResourceTypes.hpp"
#include "Utility.hpp"

#include <SFML/Window/Mouse.hpp>

#include <cmath>

//#define TRACE_ENABLE
#include "TraceLog.hpp"

#include "Settings.hpp"
#define DONE true
#define NOT_DONE false

RollingWindow::RollingWindow(fw::State::Context context)
    : mBaseWindow(*(context.window))
    , mPaint()
    , mOutput()
    , mRollingBounds()
    , mWindowSprite()
    , mBgSprite()
    , mBgParticles(ParticleSystem::Circle, BG_PARTICLE_QT, *(context.textures))
    , mParticleSys(ParticleSystem::Star, ANIM_PARTICLE_QT_HIGH, *(context.textures))
    , mBloomEnable(false)
    , mIsActionCompleted(false)
    , mIsRequestStop(false)
    , mPrevState(RollingWindow::OFF)
    , mState(mPrevState)
    , mAniMode(REFRESH_MODE)
    , mDistBuf(0.f)
    , mDistBuf2(0.f)
    , mResultStr(mNextStr)
    , mHasShowedContent(false)
{
    mPaint.create(mBaseWindow.getSize().x, mBaseWindow.getSize().y * RWIN_WIN_SCALE);
    mOutput.create(mPaint.getSize().x, mPaint.getSize().y);
    setNextStr(INIT_MESSAGE);

    mText.setFont((*(context.fonts)).get(Fonts::Digit));
    mText.setCharacterSize(RWIN_TEXT_SIZE);
    mText.setScale(RWIN_TEXT_SCALE, RWIN_TEXT_SCALE);
    mText.setColor(RWIN_TEXT_COLOR_NORMAL);
    mText.setString(mNextStr);
    Graphics::centerOrigin(mText);

    mRollingEntity.setDrawableObj(mText);

    mEntityBounds = mText.getGlobalBounds();

    mRollingBounds = sf::FloatRect(0.f, -.5f * mEntityBounds.height, mPaint.getSize().x,
                                    mPaint.getSize().y + mEntityBounds.height);

    sf::Texture& texture = (*(context.textures)).get(Textures::RollingWinBackground);
    texture.setRepeated(true);
    mBgSprite.setTexture(texture);
    mBgSprite.setScale((float)mPaint.getSize().x / texture.getSize().x,
        (float)mPaint.getSize().y / texture.getSize().y);
    //mBgSprite.setColor(sf::Color(16, 20, 24, 250));
    mBgSprite.setColor(sf::Color(mBgSprite.getColor().r, 
                                 mBgSprite.getColor().g,
                                 mBgSprite.getColor().b, 25));
    Graphics::centerOrigin(mBgSprite);
    mBgSprite.setPosition(.5f * mPaint.getSize().x, .5f * mPaint.getSize().y);

    mIsBloomSupported = PostEffect::isSupported();

    mWindowSprite.setTexture(mOutput.getTexture());
    Graphics::centerOrigin(mWindowSprite);
    mPendingCMDsQueue.clear();
}

void RollingWindow::update(sf::Time dt)
{
    if (mIsActionCompleted) { processNextCMD(); }

    switch (mState)
    {
    case OFF:
        mRollingEntity.setPosition(.5f * mPaint.getSize().x, -getSize().y);
        mIsActionCompleted = true;
        mPrevState = mState;
        break;

    case ON_INIT:
        if(mPrevState == OFF)
        {
            /* updating transition from init position to start position */
            mIsActionCompleted = onInit(dt);
            if(mIsActionCompleted)
            {
                mBgParticles.setEmitter(sf::Vector2f(getSize().x, getSize().y + 30.f), ParticleSystem::FloatUp);
                mBgParticles.setEmitSpeed(BG_PARTICLE_SPEED);
                mBgParticles.setLifeTime(BG_PARTICLE_TIME);
                mBgParticles.emit(true);

                /* done moving, change current state to ON_READY */
                mPrevState = mState;
                mState = ON_READY;
                TRACE("RollingWindow.cpp", "Initialization done, Transitting to state ON_READY...");
            }
        }
        break;

    case ON_READY:
        /* simply do nothing for now */
        mIsActionCompleted = true;
        mEntityStatus = EntityStatus::STOPPED;

        if (mPrevState == ON_RUN || mPrevState == ON_RUN_LOCKED)
        {
            mParticleSys.setEmitter(sf::Vector2f(getPosition().x, 150.f), ParticleSystem::Drop);
            mParticleSys.setEmitSpeed(ANIM_PARTICLE_SPEED);
            mParticleSys.setLifeTime(ANIM_PARTICLE_TIME);
            mParticleSys.emit(true);
            mPrevState = mState;
        }
        break;

    case ON_RUN:
    case ON_RUN_LOCKED:
        onRun(dt);
        break;

    case ON_RESTART:
        /* roll one round, reset numbers to zeros */
        if (onRestart(dt))
        {
            mIsActionCompleted = true;
            mPrevState = mState;
            mState = ON_READY;
            TRACE("GameScreen.cpp", "Restart done, Transitting to state ON_READY...");
        }
        break;

    default:
        break;
    }

    mRollingEntity.update(dt);
    mParticleSys.update(dt);
    mBgParticles.update(dt);
}

void RollingWindow::draw()
{
    if (mBloomEnable && mIsBloomSupported)
    //if (false)
    {
        mPaint.clear();
        mPaint.draw(mBgSprite);
        mPaint.draw(mRollingEntity);
        mPaint.draw(mBgParticles);
        mPaint.draw(mParticleSys);
        mPaint.display();
        
        mBloomEffect.apply(mPaint,mOutput);
        mOutput.display();
        mWindowSprite.setTexture(mOutput.getTexture());
        mBaseWindow.draw(mWindowSprite);
    }
    else
    {
        mPaint.clear();
        mPaint.draw(mBgSprite);
        mPaint.draw(mRollingEntity);
        mPaint.draw(mBgParticles);
        mPaint.draw(mParticleSys);
        mPaint.display();
        mWindowSprite.setTexture(mPaint.getTexture());
        mBaseWindow.draw(mWindowSprite);
    }
}

void RollingWindow::setSize(unsigned int width, unsigned int height)
{
    mPaint.create(width, height);
}

void RollingWindow::setPosition(float posX, float posY)
{
    mWindowSprite.setPosition(posX, posY);
}

void RollingWindow::setBloomEnable(bool flag)
{
    mBloomEnable = flag;
}

bool RollingWindow::requestOn()
{
    if(mState == RollingWindow::OFF)
    {
        TRACE("GameScreen.cpp", "requesting TURN_ON...");
        mPendingCMDsQueue.push_back(CMD::TURN_ON);
        return true;
    }
    return false;
}

bool RollingWindow::requestRun()
{
    if(mState == RollingWindow::ON_READY)
    {
        TRACE("GameScreen.cpp", "requesting RUN...");
        mPendingCMDsQueue.push_back(CMD::RUN);
        return true;
    }
    return false;
}

bool RollingWindow::requestRestart(const std::string& str)
{
    if(mState == RollingWindow::ON_READY)
    {
        TRACE("GameScreen.cpp", "requesting RESTART...");
        mPendingCMDsQueue.push_back(CMD::RESTART);
        setResultStr(str);
        return true;
    }
    return false;
}

bool RollingWindow::requestStop()
{
    if(mState == RollingWindow::ON_RUN)
    {
        TRACE("GameScreen.cpp", "requesting TURN_STOP...");
        mPendingCMDsQueue.push_back(CMD::STOP);
        return true;
    }
    return false;
}

bool RollingWindow::requestOff()
{
    if(mState == RollingWindow::ON_READY)
    {
        TRACE("GameScreen.cpp", "requesting TURN_OFF...");
        mPendingCMDsQueue.push_back(CMD::TURN_OFF);
        return true;
    }
    return false;
}

bool RollingWindow::requestChangeMode(int mode)
{
    if (mState == RollingWindow::ON_READY
        || mState == RollingWindow::OFF)
    {
        TRACE("GameScreen.cpp", "Changing mode...");
        mAniMode = mode;
        return true;
    }
    return false;
}

void RollingWindow::setNextStr(const std::string& str)
{
    mHasShowedContent = false;
    if (isStringOfNum(str))
    {
        mNextStr = insertSpaces(2, str);
    }
    else { mNextStr = str; }
}

void RollingWindow::setResultStr(const std::string& str)
{
    if (isStringOfNum(str))
    {
        mResultStr = insertSpaces(2, str);
    }
    else { mResultStr = str; }
}

bool RollingWindow::onInit(sf::Time dt)
{
    float destPosY = this->getSize().y * .5f;
    float currPosY = mRollingEntity.getPosition().y;
    if (currPosY == destPosY) { return DONE; }

    // calculate distance remains
    mDistBuf = destPosY - currPosY;
    float vy = std::abs(mRollingEntity.getVelocity().y);
    float dir = mDistBuf / std::abs(mDistBuf);

    if (std::abs(mDistBuf) <= vy * dt.asSeconds())
    {
        mRollingEntity.setVelocity(0.f, 0.f);
        mRollingEntity.setPosition(mRollingEntity.getPosition().x, destPosY);
        mDistBuf = 0.f;

        return DONE;
    }
    else if (vy > 0 && vy <= RWIN_MIN_SPEED)
    {
        vy = RWIN_MIN_SPEED;
    }
    else
    {
        vy = 5.f * mDistBuf;
    }
    mRollingEntity.setVelocity(0.f, vy * dir);
    return NOT_DONE;
}

bool RollingWindow::onRun(sf::Time dt)
{
    if (mAniMode == SLIDE_MODE)
    {
        if (onRunSlideMode(dt) && mEntityStatus == EntityStatus::STOPPED)
        {
            mIsRequestStop = false;
            mIsActionCompleted = true;
            mPrevState = mState;
            mState = ON_READY;
            TRACE("GameScreen.cpp", "Rolling Stop, Transitting to state ON_READY...");
            return DONE;
        }

        if (!mIsRequestStop && mEntityStatus == REACHED_MAX_SPEED)
        {
            mState = ON_RUN;
            mIsActionCompleted = true;
        }
        else { mIsActionCompleted = false; }
        return NOT_DONE;
    }
    else
    {
        mIsActionCompleted = true;
        mState = ON_RUN;
        if (onRunRefreshMode(dt) && mEntityStatus == EntityStatus::STOPPED)
        {
            mIsRequestStop = false;

            mPrevState = mState;
            mState = ON_READY;
            return DONE;
        }
        return NOT_DONE;
    }
}

bool RollingWindow::onRestart(sf::Time dt)
{
    float destY = 0.5f * getSize().y;
    float curY = mRollingEntity.getPosition().y;
    float vy = mRollingEntity.getVelocity().y;
    float dy;
    if(curY < destY)
    {
        dy = destY - curY;

        /* decelerate option */
        //vy = 15.f * dy;
        //if (vy < RWIN_MIN_SPEED-60) vy = RWIN_MIN_SPEED-60;

        if(dy < vy * dt.asSeconds())
        {
            mRollingEntity.setVelocity(0.f, 0.f);
            mRollingEntity.setPosition(mRollingEntity.getPosition().x, destY);
            return DONE;
        }
        mRollingEntity.setVelocity(0.f, vy);
        return NOT_DONE;
    }
    else if(curY >= destY && vy < RWIN_MAX_SPEED)
    {
        /* accelerate option */
        //pItem->mEntity->accelerate(0.f, 10.f * RWIN_ACCELERATE_SPEED);
        mRollingEntity.setVelocity(0.f, RWIN_MAX_SPEED);
    }

    dy = curY - (getSize().y + 0.5f * mRollingBounds.height);
    if(dy > 0)
    {
        mRollingEntity.setPosition(mRollingEntity.getPosition().x,
                                      dy + mRollingBounds.top);
        mText.setString(mResultStr);
        Graphics::centerOrigin(mText);
    }

    return NOT_DONE;
}

void RollingWindow::processNextCMD()
{
    if (mPendingCMDsQueue.empty()) return;
    else
    {
        switch (mPendingCMDsQueue.front())
        {
        case RUN:
            mIsRequestStop = false;
            mState = RollingWindow::ON_RUN_LOCKED;
            break;

        case STOP:
            mIsRequestStop = true;
            mState = RollingWindow::ON_RUN_LOCKED;
            break;

        case RESTART:
            mState = RollingWindow::ON_RESTART;
            setNextStr(mResultStr);
            break;

        case TURN_ON:
            mState = RollingWindow::ON_INIT;
            break;

        case TURN_OFF:
            mState = RollingWindow::OFF;
            break;

        default:
            break;
        }
        mPendingCMDsQueue.pop_front();
    }
}

bool RollingWindow::onRunSlideMode(sf::Time dt)
{
    if (mIsRequestStop && mDistBuf != 0)
    {
        float vy = RWIN_MIN_SPEED + (mDistBuf / mDistBuf2) * (RWIN_MAX_SPEED - RWIN_MIN_SPEED);
        mRollingEntity.setVelocity(0.f, vy);
        mEntityStatus = EntityStatus::DECELERATING;

        float nextMove = mRollingEntity.getVelocity().y * dt.asSeconds();
        if (mDistBuf <= nextMove)
        {
            mRollingEntity.setPosition(mRollingEntity.getPosition().x,
                0.5f * getSize().y);
            mRollingEntity.setVelocity(0.f, 0.f);
            mDistBuf = 0.f;
            mDistBuf2 = 0.f;
            mEntityStatus = EntityStatus::STOPPED;
            //mIsRequestStop = false;
            return DONE;
        }
        mDistBuf -= nextMove;
    }
    else if (mRollingEntity.getVelocity().y < RWIN_MAX_SPEED)
    {
        mRollingEntity.accelerate(0.f, RWIN_ACCELERATE_SPEED);
        if (mRollingEntity.getVelocity().y >= RWIN_MAX_SPEED)
        {
            mRollingEntity.setVelocity(0.f, RWIN_MAX_SPEED);
            mEntityStatus = EntityStatus::REACHED_MAX_SPEED;
        }
        else { mEntityStatus = EntityStatus::ACCELERATING; }
    }

    float dy = mRollingEntity.getPosition().y
        - (getSize().y + 0.5f * mEntityBounds.height);
    if (dy > 0)
    {
        mRollingEntity.setPosition(mRollingEntity.getPosition().x,
            mRollingBounds.top + dy);

        if (mIsRequestStop)
        {
            if (mDistBuf == 0)
            {
                // set decceleration distanced
                mDistBuf = 0.5f * getSize().y - mRollingEntity.getPosition().y;
                mDistBuf += 5.f * mRollingBounds.height - RWIN_ERROR_MARGIN;
                mDistBuf2 = mDistBuf;
            }
            else if (mDistBuf > 0 && mDistBuf <= mRollingBounds.height)
            {
                mNextStr = mResultStr;
            }
        }

        mText.setString(mNextStr);
        Graphics::centerOrigin(mText);
        mHasShowedContent = true;
    }

    return NOT_DONE;
}

bool RollingWindow::onRunRefreshMode(sf::Time dt)
{
    if (mIsRequestStop)
    {
        mText.setString(mResultStr);
        Graphics::centerOrigin(mText);
        mTimeBuf = 0.f;
        mEntityStatus = EntityStatus::STOPPED;
        return DONE;
    }
    else
    {
        if (mTimeBuf > RWIN_REFRESH_TIME)
        {
            mText.setString(mNextStr);
            Graphics::centerOrigin(mText);
            mHasShowedContent = true;
            mTimeBuf = 0.f;
            mEntityStatus = EntityStatus::REACHED_MAX_SPEED;
        }
        mTimeBuf += dt.asSeconds();
        return NOT_DONE;
    }
}

void RollingWindow::switchClbAnim()
{
    if (mParticleSys.isAllDismissed())
    {
        if (mParticleSys.getParticleType() == ParticleSystem::Star)
        {
            mParticleSys.selectParticle(ParticleSystem::Gold);
            mParticleSys.setParticleQt(ANIM_PARTICLE_QT);
        }
        else
        {
            mParticleSys.selectParticle(ParticleSystem::Star);
            mParticleSys.setParticleQt(ANIM_PARTICLE_QT_HIGH);
        }
    }
}

bool RollingWindow::isRunning() const
{
    return !(mState == OFF || mEntityStatus == EntityStatus::STOPPED && mState == ON_READY);
}

bool RollingWindow::isBloomEnable() const
{
    return mBloomEnable;
}

bool RollingWindow::hasShowedContent() const
{
    return mHasShowedContent;
}

int RollingWindow::getCurrentMode() const
{
    return mAniMode;
}

sf::Vector2f RollingWindow::getPosition() const
{
    return mWindowSprite.getPosition();
}

sf::Vector2f RollingWindow::getSize() const
{
    return sf::Vector2f(mWindowSprite.getLocalBounds().width,
        mWindowSprite.getLocalBounds().height);
}





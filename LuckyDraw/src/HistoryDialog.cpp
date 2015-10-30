#include "HistoryDialog.h"

#include "ResourceHolder.hpp"
#include "ResourceTypes.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#define DIALOG_WIDTH 360.f
#define DIALOG_HEIGHT 400.f
#define DIALOG_TEXT_SIZE 30
#define DIALOG_TITLE_SIZE 42

#define LINE_HEIGHT_MARGIN 5.f

#define DIALOG_TITLE_COLOR sf::Color(54, 215, 183, 255)
#define DIALOG_TEXT_COLOR sf::Color(255, 255, 255, 255)
//#define DIALOG_BG_COLOR sf::Color(54, 215, 183, 200)
#define DIALOG_BG_COLOR sf::Color(24, 39, 43, 240)
//#define DIALOG_BG_COLOR sf::Color(0, 0, 0, 200)

#define MOVEMENT_SPEED 1200.f

HistoryDialog::HistoryDialog(const std::string& info,
                             const fw::State::Context& context)
: mWin(*(context.window)),
  mFrame(sf::Vector2f(DIALOG_WIDTH, DIALOG_HEIGHT)),
  mTitle(L"选中名单", context.fonts->get(Fonts::Chinese), DIALOG_TITLE_SIZE),
  mContent(info, context.fonts->get(Fonts::Chinese), DIALOG_TEXT_SIZE),
  mTopIndex(0),
  mBotIndex(0),
  p_mArrowUp(nullptr),
  p_mArrowDown(nullptr),
  mIsDisplay(false)
{
    mDisplayPos.x = .5f * context.window->getView().getSize().x;
    mDisplayPos.y = mFrame.getGlobalBounds().height * .5f + 20.f;
    mBeginPos.x = mDisplayPos.x;
    mBeginPos.y = -.5f * mFrame.getGlobalBounds().height;

    setPosition(mBeginPos);

    SimpleButton::Ptr arrow_up(new SimpleButton(14));
    SimpleButton::Ptr arrow_down(new SimpleButton(14));
    p_mArrowUp = arrow_up.get();
    p_mArrowDown = arrow_down.get();
    attachChild(std::move(arrow_up));
    attachChild(std::move(arrow_down));

    p_mArrowDown->setCallback([this]()
    {
        strScrollDown();
    });

    p_mArrowUp->setCallback([this]()
    {
        strScrollUp();
    });

    mFrame.setFillColor(DIALOG_BG_COLOR);
    mTitle.setColor(DIALOG_TITLE_COLOR);
    mContent.setColor(DIALOG_TEXT_COLOR);
    p_mArrowUp->setButtonColor(DIALOG_TITLE_COLOR);
    p_mArrowDown->setButtonColor(DIALOG_TITLE_COLOR);

    p_mArrowDown->setRotation(180.f);
    p_mArrowDown->setButtonSize(16.f, 26.f);
    p_mArrowUp->setButtonSize(16.f, 26.f);

    maxNoLines = getMaxNoLineToShow();
    pack();
}

HistoryDialog::~HistoryDialog()
{
}

void HistoryDialog::update(sf::Time dt)
{
    if (mTopIndex <= 0) { p_mArrowUp->setFocusable(false); }
    else { p_mArrowUp->setFocusable(true); }

    if (mBotIndex <= 0 || mBotIndex >= (int)mStrVec.size() - 1) { p_mArrowDown->setFocusable(false); }
    else { p_mArrowDown->setFocusable(true); }

    p_mArrowUp->update(mWin);
    p_mArrowDown->update(mWin);
    if (mIsDisplay)
    {
        onMove(dt, mDisplayPos);
    }
    else
    {
        onMove(dt, mBeginPos);
    }

    EntityNode::update(dt);
}

void HistoryDialog::onDraw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(mFrame, state);
    target.draw(mTitle, state);
    target.draw(mContent, state);
}

void HistoryDialog::display()
{
    mIsDisplay = true;
}
void HistoryDialog::dismiss()
{
    mIsDisplay = false;
}

void HistoryDialog::clearContent()
{
    mContent.setString("");
}

void HistoryDialog::setContent(const sf::String& info)
{
    mStrVec.clear();
    strToVec(mStrVec, info);
    if (mStrVec.empty()) { return; }

    mBotIndex = mStrVec.size() - 1;
    mTopIndex = mStrVec.size() - maxNoLines;
    if (mTopIndex < 0) { mTopIndex = 0; }

    sf::String str;
    for (int i = mTopIndex; i <= mBotIndex; ++i)
    {
        str += mStrVec[i];
    }
    mContent.setString(str);

    pack();
}

bool HistoryDialog::isDisplay() const
{
    return mIsDisplay;
}


bool HistoryDialog::onMove(sf::Time dt, sf::Vector2f dest)
{
    if (getPosition() != dest)
    {
        sf::Vector2f s = dest - getPosition();
        sf::Vector2f dir = unitVector(s);
        setVelocity(dir * MOVEMENT_SPEED);
        if (magnitudeOf(getVelocity() * dt.asSeconds()) > magnitudeOf(s))
        {
            setVelocity(0.f, 0.f);
            setPosition(dest);
            return true;
        }
        return false;
    }
    else
    {
        setVelocity(0.f, 0.f);
        return true;
    }
}

void HistoryDialog::strScrollDown()
{
    if (mBotIndex >= (int)mStrVec.size() - 1) { return; }

    mTopIndex++;
    mBotIndex++;

    sf::String str;
    for (int i = mTopIndex; i <= mBotIndex; ++i)
    {
        str += mStrVec[i];
    }

    mContent.setString(str);
}

void HistoryDialog::strScrollUp()
{
    if (mTopIndex <= 0) { return; }

    mTopIndex--;
    mBotIndex--;

    sf::String str;
    for (int i = mTopIndex; i <= mBotIndex; ++i)
    {
        str += mStrVec[i];
    }

    mContent.setString(str);
}

void HistoryDialog::pack()
{
    Graphics::centerOrigin(mFrame);
    Graphics::centerOrigin(mTitle);
    Graphics::centerOrigin(mContent);

    mTitle.setPosition(0.f, mFrame.getGlobalBounds().top
        + mTitle.getGlobalBounds().height);

    mContent.setPosition(0.f, mFrame.getGlobalBounds().top
        + mTitle.getGlobalBounds().height * 2.f
        + mContent.getGlobalBounds().height * .5f);

    p_mArrowUp->setPosition(mFrame.getGlobalBounds().left
                          + mFrame.getGlobalBounds().width
                          - p_mArrowUp->getWidth() * 2.f
                          , mFrame.getPosition().y
                          - p_mArrowUp->getHeight() * 1.f);

    p_mArrowDown->setPosition(mFrame.getGlobalBounds().left
                          + mFrame.getGlobalBounds().width
                          - p_mArrowDown->getWidth() * 2.f
                          , mFrame.getPosition().y
                          + p_mArrowUp->getHeight() * 1.f);


}

int HistoryDialog::getMaxNoLineToShow()
{
    float max_height = mFrame.getGlobalBounds().height
                        - mTitle.getGlobalBounds().height * 3.f;


    sf::Text line(mContent);
    line.setString("0000");
    float line_height = line.getGlobalBounds().height + LINE_HEIGHT_MARGIN;
    return (int)std::floor((max_height / line_height));
}

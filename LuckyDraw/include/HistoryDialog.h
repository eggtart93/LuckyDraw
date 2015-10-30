#pragma once

#include "State.h"
#include "EntityNode.h"
#include "SimpleButton.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/String.hpp>

using namespace GUI;

class HistoryDialog : public EntityNode
{
    public:
    typedef std::unique_ptr<HistoryDialog> Ptr;

    public:
    HistoryDialog(const std::string& info, const fw::State::Context& context);
    ~HistoryDialog();

    virtual void update(sf::Time dt);
    virtual void onDraw(sf::RenderTarget& target, sf::RenderStates state) const;

    void display();
    void dismiss();

    void clearContent();
    void setContent(const sf::String& info);
    void appendText(const sf::String& info);

    bool isDisplay() const;

    private:
    void pack();
    bool onMove(sf::Time dt, sf::Vector2f dest);
    void strScrollDown();
    void strScrollUp();
    void trimTextContent();
    int getMaxNoLineToShow();

    private:
    const sf::RenderWindow& mWin;
    sf::RectangleShape mFrame;
    sf::Text mTitle;
    sf::Text mContent;
    std::vector<sf::String> mStrVec;
    int mTopIndex;
    int mBotIndex;
    SimpleButton* p_mArrowUp;
    SimpleButton* p_mArrowDown;
    sf::Vector2f mDisplayPos;
    sf::Vector2f mBeginPos;
    int maxNoLines;
    bool mIsDisplay;
};


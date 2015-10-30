#include "Button.h"
#include "Utility.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/String.hpp>


#include "TraceLog.hpp"

namespace GUI
{
#define BUTTON_TEXT_SIZE 12
#define BUTTON_WIDTH 135
#define BUTTON_HEIGHT 34
#define OUT_LINE_THICKNESS 2.f

#define TRANS_EFFECT_LIGHT 0.90f
#define RELEASED_TRANS_EFFECT 0.9f
#define FOCUSED_TRANS_EFFECT 1.f
#define PRESSED_TRANS_EFFECT 0.65f

#define TEXT_COLOR_RELEASED sf::Color(255,255,255,255*TRANS_EFFECT_LIGHT)
#define TEXT_COLOR_FOCUSED sf::Color(255,255,255,255*FOCUSED_TRANS_EFFECT)
#define TEXT_COLOR_PRESSED sf::Color(255,255,255,255*PRESSED_TRANS_EFFECT)
#define TEXT_COLOR_DEFAULT sf::Color(0,0,0,255)

#define TEXT_BG_COLOR_RELEASED sf::Color(52, 73, 94, 0)
#define TEXT_BG_COLOR_FOCUSED sf::Color(52, 73, 94, 255)
//#define TEXT_BG_COLOR_FOCUSED sf::Color(175, 255, 217, 255)
#define TEXT_BG_COLOR_PRESSED sf::Color(52, 73, 94, 255*PRESSED_TRANS_EFFECT)
#define OUT_LINE_COLOR sf::Color(52, 73, 94, 255)

#define COLOR_ON_RELEASED sf::Color(0,0,0,255*RELEASED_TRANS_EFFECT)
#define COLOR_ON_FOCUSED sf::Color(0,0,0,255)
#define COLOR_ON_PRESSED sf::Color(0,0,0,255*PRESSED_TRANS_EFFECT)

    Button::Button(ButtonStyle style, const FontHolder& fonts, const TextureHolder& textures)
    : mStyle(style),
      mFonts(fonts),
      mTextures(textures),
      mIsFocusable(true),
      mIsFixedSize(true),
      mCallback(),
      p_mShape(nullptr),
      p_mSprite(nullptr),
      p_mText(nullptr),
      mColor(COLOR_ON_RELEASED),
      mTextColor(TEXT_COLOR_DEFAULT),
      mScale(1.f, 1.f),
      mState(State::OnReleased)
    {
        if (mStyle == Button::Sprite)
        {
            std::unique_ptr<sf::Sprite> p_sprite(new sf::Sprite());
            p_sprite->setTexture(textures.get(Textures::Button));
            Graphics::centerOrigin(*p_sprite);
            p_mSprite = std::move(p_sprite);
            setText("");
        }
        else
        {
            std::unique_ptr<sf::RectangleShape> p_rect(
                new sf::RectangleShape(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT)));
            Graphics::centerOrigin(*p_rect);
            p_mShape = std::move(p_rect);

            if (mStyle == ButtonStyle::Text)
            {
                std::unique_ptr<sf::Text> p_text(new sf::Text());
                p_text->setFont(fonts.get(Fonts::Chinese));
                //p_text->setCharacterSize(30);
                p_text->setColor(mTextColor);
                p_text->setString("Button");
                Graphics::centerOrigin(*p_text);
                
                p_mShape->setFillColor(TEXT_BG_COLOR_RELEASED);
                p_mShape->setOutlineThickness(OUT_LINE_THICKNESS);
                p_mShape->setOutlineColor(mTextColor);

                p_mText = std::move(p_text);
            }

            setText("");
        }
    }
    uint16_t Button::getWidth() const
    {
        if (mStyle == Button::Shape || mStyle == Button::Text)
            return (uint16_t)(p_mShape->getGlobalBounds().width);
        else if (mStyle == Button::Sprite)
            return (uint16_t)(p_mSprite->getGlobalBounds().width);
        else return 0;
    }

    uint16_t Button::getHeight() const
    {
        if (mStyle == Button::Shape || mStyle == Button::Text)
            return (uint16_t)(p_mShape->getGlobalBounds().height);
        else if (mStyle == Button::Sprite)
            return (uint16_t)(p_mSprite->getGlobalBounds().height);
        else return 0;
    }


    bool Button::isFocusable() const
    {
        return mIsFocusable;
    }

    void Button::update(const sf::RenderWindow& window)
    {
        if (!isFocusable())
        {
            onLock();
            updateAppearance();
            return;
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        bool isMouseOn = mousePos.x >= (getGlobalPosition().x - getWidth() * .5f)
                        && mousePos.x <= (getGlobalPosition().x + getWidth() * .5f)
                        && mousePos.y >= (getGlobalPosition().y - getHeight() * .5f)
                        && mousePos.y <= (getGlobalPosition().y + getHeight() * .5f);
        if (isMouseOn)
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                onPress();
            }
            else
            {
                if(mState == Button::OnPressed) { onClick(); }
                onFocus();
            }
        }
        else
        {
            lostFocus();
        }
        updateAppearance();
    }

    void Button::setFocusable(bool flag)
    {
        mIsFocusable = flag;
    }

    void Button::setText(const sf::String& text)
    {
        if (!p_mText)
        {
            std::unique_ptr<sf::Text> p_text(new sf::Text());
            p_text->setFont(mFonts.get(Fonts::Chinese));
            p_text->setColor(mTextColor);
            //p_text->setCharacterSize(30);
            p_mText = std::move(p_text);
        }
        p_mText->setString(text);
        Graphics::centerOrigin(*p_mText);
    }

    void Button::setTextColor(sf::Color color)
    {
        mTextColor = color;
    }

    void Button::setTextSize(float s)
    {
        if(p_mText)
        {
            p_mText->setCharacterSize(s);
            Graphics::centerOrigin(*p_mText);
        }
    }

    void Button::setButtonColor(sf::Color color)
    {
        mColor = color;
    }

    void Button::setButtonSize(float w, float h)
    {
        if (mStyle == ButtonStyle::Text)
        {
            p_mShape->setSize(sf::Vector2f(w, h));
            Graphics::centerOrigin(*p_mShape);
        }
        else if (mStyle == Button::Sprite)
        {
            if(!p_mSprite)
            {
                TRACE("Button.cpp", "ERROR: p_mSprite is nullptr");
                return;
            }
            mScale.x = w / p_mSprite->getGlobalBounds().width;
            mScale.y = h / p_mSprite->getGlobalBounds().height;
            p_mSprite->setScale(mScale);
        }
        else if (mStyle == Button::Shape)
        {
            if(!p_mShape)
            {
                TRACE("Button.cpp", "ERROR: p_mShape is nullptr");
                return;
            }
            mScale.x = w / p_mShape->getGlobalBounds().width;
            mScale.y = h / p_mShape->getGlobalBounds().height;
            p_mShape->setScale(mScale);
        }
    }

    void Button::warp()
    {
        if(mIsFixedSize) return;

        if(p_mShape)
        {
            p_mShape->setSize(sf::Vector2f(p_mText->getGlobalBounds().width + 4.f,
                                       p_mText->getGlobalBounds().height + 2.f));
        }
        else if(p_mSprite)
        {
            setButtonSize(p_mText->getGlobalBounds().width + 4.f,
                    p_mText->getGlobalBounds().height + 2.f);
        }
    }

    void Button::setCallback(Callback callback)
    {
        mCallback = std::move(callback);
    }

    void Button::onFocus()
    {
        mState = Button::OnFocused;
    }

    void Button::lostFocus()
    {
        mState = Button::OnReleased;
    }

    void Button::onPress()
    {
        mState = Button::OnPressed;
    }

    void Button::onClick()
    {
        if(mCallback) { mCallback(); }
    }

    void Button::onLock()
    {
        mState = Button::OnLocked;
    }

    Button::State Button::getState() const
    {
        return mState;
    }

    void Button::onDraw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (mStyle == Button::Sprite)
        {
            target.draw(*p_mSprite, states);
        }
        else
        {
            target.draw(*p_mShape, states);
            target.draw(*p_mText, states);
        }
    }

    void Button::updateAppearance()
    {
        sf::Color color = mColor;
        switch(mState)
        {
        case OnLocked:
            break;
        case OnReleased:
            if (mStyle == ButtonStyle::Shape)
            {
                color.a *= RELEASED_TRANS_EFFECT;
                p_mShape->setFillColor(color);
                p_mShape->setScale(1.f, 1.f);
                //p_mShape->setSize(sf::Vector2f(mWidth, mHeight));
                p_mText->setColor(mTextColor);
            }
            else if (mStyle == ButtonStyle::Sprite)
            {
                p_mSprite->setScale(mScale);
                p_mText->setColor(mTextColor);
            }
            else
            {
                p_mText->setScale(mScale.x, mScale.y);
                p_mText->setColor(mTextColor);
                p_mShape->setFillColor(TEXT_BG_COLOR_RELEASED);
                p_mShape->setOutlineThickness(OUT_LINE_THICKNESS);
                p_mShape->setOutlineColor(mTextColor);
            }
            p_mText->setStyle(sf::Text::Regular);
            break;

        case OnFocused:
            if (mStyle == ButtonStyle::Shape)
            {
                color.a *= FOCUSED_TRANS_EFFECT;
                p_mShape->setFillColor(color);
                p_mShape->setScale(mScale.x + .1f, mScale.y + .05f);
                //p_mShape->setSize(sf::Vector2f(1.1f * mWidth, 1.05f * mHeight));
                p_mText->setColor(TEXT_COLOR_FOCUSED);
            }
            else if (mStyle == ButtonStyle::Sprite)
            {
                p_mSprite->setScale(mScale.x + .1f, mScale.y + .1f);
                p_mText->setColor(TEXT_COLOR_FOCUSED);
            }
            else
            {
                p_mText->setColor(TEXT_COLOR_FOCUSED);
                p_mText->setScale(mScale.x + .1f, mScale.y + .1f);
                p_mShape->setFillColor(mColor);
                p_mShape->setOutlineThickness(0);
                //p_mText->setStyle(sf::Text::Bold);
            }
            break;

        case OnPressed:
            if (mStyle == ButtonStyle::Shape)
            {
                color.a *= PRESSED_TRANS_EFFECT;
                p_mShape->setFillColor(color);
                p_mShape->setScale(mScale.x - .1f, mScale.y - .1f);
                //p_mShape->setSize(sf::Vector2f(.9f * mWidth, .9f * mHeight));
                p_mText->setColor(TEXT_COLOR_PRESSED);
            }
            else if (mStyle == ButtonStyle::Sprite)
            {
                p_mSprite->setScale(mScale.x - .1f, mScale.y - .1f);
                p_mText->setColor(TEXT_COLOR_PRESSED);
            }
            else
            {
                p_mText->setColor(TEXT_COLOR_PRESSED);
                p_mText->setScale(mScale.x - .1f, mScale.y - .1f);
                p_mText->setStyle(sf::Text::Bold);
                p_mShape->setFillColor(TEXT_BG_COLOR_PRESSED);
                p_mShape->setOutlineThickness(0);
            }
            break;

        default:
            break;
        }
    }
}

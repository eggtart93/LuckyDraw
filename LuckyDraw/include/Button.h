#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include "ResourceHolder.hpp"
#include "ResourceTypes.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace GUI
{
class Button : public Component
{
public:
    typedef std::unique_ptr<Button> Ptr;
    typedef std::function<void()> Callback;

    enum State
    {
        OnLocked,
        OnReleased,
        OnFocused,
        OnPressed,
        StateCount
    };

    enum ButtonStyle
    {
        Shape,
        Sprite,
        Text
    };

public:
    Button(ButtonStyle style, const FontHolder& fonts, const TextureHolder& textures);

    virtual uint16_t getWidth() const;
    virtual uint16_t getHeight() const;
    virtual bool isFocusable() const;
    virtual void update(const sf::RenderWindow& window);

    void setCallback(Callback callback);
    void setFont(const sf::Font& font);
    void setFocusable(bool flag);
    void setText(const sf::String& text);
    void setTextColor(sf::Color color);
    void setTextSize(float h);

    void setButtonColor(sf::Color color);
    void setButtonSize(float width, float height);
    void warp();


protected:
    virtual void onFocus();
    virtual void lostFocus();
    virtual void onPress();
    virtual void onClick();
    virtual void onLock();
    State getState() const;

private:
    virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateAppearance();

protected:
    ButtonStyle mStyle;
    const FontHolder& mFonts;
    const TextureHolder& mTextures;
    bool mIsFocusable;
    bool mIsFixedSize;
    Callback mCallback;
    std::unique_ptr<sf::RectangleShape> p_mShape;
    std::unique_ptr<sf::Sprite> p_mSprite;
    std::unique_ptr<sf::Text> p_mText;
    sf::Color mColor;
    sf::Color mTextColor;
    sf::Vector2f mScale;
    State mState;
};

}
#endif // BUTTON_H

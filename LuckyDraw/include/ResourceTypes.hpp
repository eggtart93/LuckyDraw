#ifndef RESOURCETYPES_HPP_INCLUDED
#define RESOURCETYPES_HPP_INCLUDED

// Forward declaration of SFML classes
namespace sf
{
    class Texture;
    class Font;
    class Shader;
    class SoundBuffer;
}

namespace Shaders
{
    enum ID
    {
        BrightnessPass,
        DownSamplePass,
        GaussianBlurPass,
        AddPass
    };
}

namespace Textures
{
    enum ID
    {
        SystemBackground,
        RollingWinBackground,
        Button,
        Coin,
        Star,
        Particle,
        Banana,
        Gold
    };
}

namespace Fonts
{
    enum ID
    {
        System,
        Digit,
        Chinese
    };
}

namespace SoundEffects
{
    enum ID
    {
        DigitChange,
        Restart,
        Celebrate,
        Notification,
        ButtonSelect,
        ButtonClick
    };
}

// Forward declaration
template <typename Resource, typename ID>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID> ShaderHolder;
#endif // RESOURCETYPES_HPP_INCLUDED

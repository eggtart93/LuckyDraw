#include "Utility.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <cmath>
#include <cassert>


void dequeToStr(std::deque<sf::String>& strBuf, sf::String& str)
{
    size_t i = 0;
    while (i < strBuf.size())
    {
        str += strBuf[i];
        ++i;
    }
}

void strToDeque(std::deque<sf::String>& strBuf, sf::String& str)
{
    sf::String line;
    size_t i = 0;
    while (i < str.getSize())
    {
        line += str[i];
        if (str[i] == '\n')
        {
            strBuf.push_back(line);
            line = "";
        }
        ++i;
    }
}

void strToVec(std::vector<sf::String>& vec, const sf::String& str)
{
    sf::String line;
    size_t i = 0;
    while (i < str.getSize())
    {
        line += str[i];
        if (str[i] == '\n')
        {
            vec.push_back(line);
            line = "";
        }
        ++i;
    }
}

void vecToStr(const std::vector<sf::String>& vec, sf::String& str)
{
    size_t i = 0;
    while (i < vec.size())
    {
        str += vec[i];
        ++i;
    }
}

bool isStringOfNum(const std::string& str)
{
    for (auto it = str.begin(); it != str.end(); ++it)
    {
        if (*it < '0' && *it > '9') { return false; }
    }
    return true;
}

int getNumOfLines(const sf::String& str)
{
    if (str.isEmpty()) { return 0; }

    unsigned int num_line = 1;
    for(unsigned int i = 0; i < str.getSize(); ++i) { if(str[i] == '\n') {num_line++;} }
    return num_line;
}

std::string insertSpaces(size_t n, const std::string& str)
{
    if (str.size() < 2) { return ""; }
    std::string new_str(str);
    size_t i = 1;
    while (i < new_str.size())
    {
        new_str.insert(i, n, ' ');
        i += (n + 1);
    }
    return new_str;
}

float magnitudeOf(sf::Vector2f vec)
{
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

sf::Vector2f unitVector(sf::Vector2f vec)
{
    assert(vec != sf::Vector2f(0.f, 0.f));
    float mod = magnitudeOf(vec);
    return sf::Vector2f(vec.x / mod, vec.y / mod);
}

namespace Graphics
{
    void centerOrigin(sf::Text& text)
    {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    }
    void centerOrigin(sf::Shape& shape)
    {
        sf::FloatRect bounds = shape.getLocalBounds();
        shape.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    }

    void centerOrigin(sf::Sprite& sprite)
    {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    }

    bool isInBounds(sf::Vector2f pos, sf::FloatRect bounds)
    {
        if( pos.x > bounds.left && pos.x < bounds.left + bounds.width
           && pos.y > bounds.top && pos.y < bounds.top + bounds.height)
        {
            return true;
        }
        else { return false; }
    }
}


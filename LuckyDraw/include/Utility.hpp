#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/String.hpp>

#include <string>
#include <sstream>
#include <deque>
#include <vector>

// Forward declaration of SFML classes
namespace sf
{
    class Text;
    class Shape;
    class Sprite;
}


template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

void strToVec(std::vector<sf::String>& vec, const sf::String& str);

void vecToStr(const std::vector<sf::String>& vec, sf::String& str);

void strToDeque(std::deque<sf::String>& strBuf, sf::String& str);

void dequeToStr(std::deque<sf::String>& strBuf, sf::String& str);

int getNumOfLines(const sf::String& str);

bool isStringOfNum(const std::string& str);

std::string insertSpaces(size_t n, const std::string& str);

float magnitudeOf(sf::Vector2f vec);

sf::Vector2f unitVector(sf::Vector2f vec);

namespace Graphics
{
    void centerOrigin(sf::Text& text);
    void centerOrigin(sf::Shape& shape);
    void centerOrigin(sf::Sprite& sprite);
    bool isInBounds(sf::Vector2f pos, sf::FloatRect bounds);
}

#endif // UTILITY_H

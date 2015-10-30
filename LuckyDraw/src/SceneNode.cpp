#include "SceneNode.h"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <algorithm>
#include <cassert>

#define TRACE(DISABLE)

SceneNode::SceneNode()
: mChildren()
, mParent(nullptr)
{}

void SceneNode::attachChild(Ptr child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
                              [&](Ptr& p){ return p.get() == &node;} );
    assert(found != mChildren.end());
    Ptr removedNode = std::move(*found);
    removedNode->mParent = nullptr;
    mChildren.erase(found);
    return removedNode;
}


/*
sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}
*/

sf::Transform SceneNode::getGlobalTransform() const
{
    sf::Transform transform = sf::Transform::Identity;
    for (const SceneNode* node = this; node != nullptr; node = node->mParent)
    {
        transform *= node->getTransform();
    }
    return transform;
}

sf::Vector2f SceneNode::getGlobalPosition() const
{
    return getGlobalTransform() * sf::Vector2f();
}


void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    // apply transform to current node
    state.transform *= getTransform();

    onDraw(target, state);

    for(auto& child : mChildren)
    {
        child->draw(target,state);
    }

    // for testing purpose
    //drawBoundingRect(target, state);
}

void SceneNode::onDraw(sf::RenderTarget& target, sf::RenderStates state) const{}

/*
// for testing purpose
void SceneNode::drawBoundingRect(sf::RenderTarget& target, sf::RenderStates state) const
{
    sf::FloatRect rect = getBoundingRect();
    sf::RectangleShape shape(sf::Vector2f(rect.width, rect.height));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color(255, 0, 0, 255));

    shape.setPosition(rect.left, rect.top);
    // or
    //Graphics::centerOrigin(shape);
    //shape.setPosition(getPosition());

    target.draw(shape);
}

*/

#ifndef SCENENODE_H
#define SCENENODE_H

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>

class SceneNode : public sf::Drawable, public sf::Transformable, public sf::NonCopyable
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;

    public:
        explicit SceneNode();

        void attachChild(Ptr child);
        Ptr detachChild(const SceneNode& node);

        //virtual sf::FloatRect getBoundingRect() const;
        sf::Transform getGlobalTransform() const;
        sf::Vector2f getGlobalPosition() const;

    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
        //void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates state) const;

    private:
        std::vector<Ptr> mChildren;
        SceneNode* mParent;
};

#endif // SCENENODE_H

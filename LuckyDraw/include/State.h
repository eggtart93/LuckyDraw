#ifndef STATE_H
#define STATE_H

#include "StateID.hpp"
#include "ResourceTypes.hpp"

#include <memory>

namespace sf
{
    class RenderWindow;
    class Event;
    class Time;
    class Image;
}

class SoundPlayer;

class StateStack;
namespace fw
{
    class State
    {
    public:
        typedef std::unique_ptr<State> Ptr;

        struct Context
        {
            Context(sf::RenderWindow& win, 
                    TextureHolder& textures, 
                    FontHolder& fonts, 
                    //SoundPlayer& sounds,
                    sf::Image& icon);
            sf::RenderWindow* window;
            TextureHolder* textures;
            FontHolder* fonts;
            //SoundPlayer* sounds;
            sf::Image* icon;
        };

    public:
        State(StateStack& stateStack, Context context);
        virtual ~State();

        virtual bool processEvent(const sf::Event& event) = 0;
        virtual bool update(sf::Time dt) = 0;
        virtual void draw() = 0;

    protected:
        void requestStackPush(States::ID id);
        void requestStackPop();
        void requestStackClear();
        const Context& getContext() const;
        Context getContext();

    private:
        StateStack* mStack;
        Context mContext;
    };
}

#endif // STATE_H

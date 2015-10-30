#ifndef STATESTACK_H
#define STATESTACK_H

#include "State.h"
#include "ResourceTypes.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
//#include <utility>
#include <functional>
#include <map>

using namespace fw;

class StateStack : private sf::NonCopyable
{
    public:
        enum Action
        {
            Push,
            Pop,
            ClearStack
        };

    public:
        explicit StateStack(State::Context context);
        void push(States::ID id);
        void pop();
        void clearStack();
        bool isEmpty() const;

        template <typename StateType>
        void registerState(States::ID id);

        void processEvent(const sf::Event& event);
        void update(sf::Time dt);
        void draw();

    private:
        struct PendingChange
        {
            explicit PendingChange(Action action, States::ID id = States::NONE);
            Action pendingAction;
            States::ID stateID;
        };

    private:
        State::Ptr createState(States::ID id);
        void applyPendingChanges();

    private:
        std::vector<State::Ptr> mStack;
        std::vector<PendingChange> mPendingQueue;
        std::map<States::ID,std::function<State::Ptr(void)>> mStatesFactory;
        State::Context mContext;

};

template <typename StateType>
void StateStack::registerState(States::ID id)
{
    mStatesFactory[id] = [this]()
                        {
                            return State::Ptr(new StateType(*this, mContext));
                        };
}

#endif // STATESTACK_H

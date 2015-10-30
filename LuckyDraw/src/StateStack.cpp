#include "StateStack.h"
#include "Utility.hpp"

#include <cassert>

StateStack::StateStack(State::Context context)
: mStack()
, mPendingQueue()
, mStatesFactory()
, mContext(context)
{}

void StateStack::push(States::ID id)
{
    mPendingQueue.push_back(PendingChange(Action::Push, id));
}

void StateStack::pop()
{
    mPendingQueue.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStack()
{
    mPendingQueue.push_back(PendingChange(Action::ClearStack));
}

bool StateStack::isEmpty() const
{
    return mStack.empty();
}

void StateStack::processEvent(const sf::Event& event)
{
    // process event from top to bottom
    for(auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if(!(*itr)->processEvent(event)) { break; }
    }
    applyPendingChanges();
}

void StateStack::update(sf::Time dt)
{
    // update states from top to bottom
    for(auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if(!(*itr)->update(dt)) { break; }
    }
    applyPendingChanges();
}

void StateStack::draw()
{
    // draw states from bottom to top
    for(auto& itr : mStack)
    {
        itr->draw();
    }
}

State::Ptr StateStack::createState(States::ID id)
{
    auto stateFound = mStatesFactory.find(id);
    assert(stateFound != mStatesFactory.end());

    return stateFound->second();
}

void StateStack::applyPendingChanges()
{
    for(auto& pendingChange : mPendingQueue)
    {
        switch(pendingChange.pendingAction)
        {
            case Action::Push:
                mStack.push_back(createState(pendingChange.stateID));
                break;
            case Action::Pop:
                mStack.pop_back();
                break;
            case Action::ClearStack:
                mStack.clear();
                break;
            default:
                break;
        }
    }
    mPendingQueue.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID id)
: pendingAction(action)
, stateID(id)
{}

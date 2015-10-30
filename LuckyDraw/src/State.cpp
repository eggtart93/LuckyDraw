#include "State.h"
#include "StateStack.h"

using namespace fw;

State::Context::Context(sf::RenderWindow& win, 
                        TextureHolder& tHolder, 
                        FontHolder& fHolder, 
                        //SoundPlayer& sPlayer,
                        sf::Image& icon)
: window(&win)
, textures(&tHolder)
, fonts(&fHolder)
//, sounds(&sPlayer)
, icon(&icon)
{}


State::State(StateStack& stateStack, Context context)
: mStack(&stateStack)
, mContext(context)
{}

State::~State(){}

void State::requestStackPush(States::ID id)
{
    mStack->push(id);
}

void State::requestStackPop()
{
    mStack->pop();
}

void State::requestStackClear()
{
    mStack->clearStack();
}

const State::Context& State::getContext() const
{
    return mContext;
}

State::Context State::getContext()
{
    return mContext;
}
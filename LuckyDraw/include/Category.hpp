#ifndef CATEGORY_HPP_INCLUDED
#define CATEGORY_HPP_INCLUDED

namespace Category
{
	enum Type
	{
		None				= 0,
		ScrollingFrame		= 1 << 0,
		ScrollingText		= 1 << 1,
		ParticleSystem		= 1 << 2,
	};
}

#endif // CATEGORY_HPP_INCLUDED

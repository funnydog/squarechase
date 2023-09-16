#include <iostream>

#include "gameview.hpp"

#include "color.hpp"
#include "rendertarget.hpp"
#include "resourceholder.hpp"
#include "texture.hpp"
#include "utility.hpp"
#include "window.hpp"

namespace
{
const float MinSize = 10.f;
const float MaxSize = 25.f;
const glm::vec2 DeltaSize(1.f);

const float MaxTimePerSquare = 2.75f;
const float MinTimePerSquare = 0.75f;
const float DeltaTime = 0.25f;

const float MaxTimeWithoutHits = 10.f;

const Color colors[] = {
	Color::Red,
	Color::Green,
	Color::Blue,
};

}

GameView::GameView(ViewStack &stack, const Context &context)
	: mViewStack(stack)
	, mContext(context)
	, mPlayerScore(0)
	, mTimeRemaining(0.f)
	, mTimePerSquare(MaxTimePerSquare)
	, mTimeWithoutHits(0.f)
	, mRectangle{{0.f, 0.f}, {MaxSize, MaxSize}}
{
}

bool
GameView::update(float dt)
{
	if (mTimeRemaining <= 0.f)
	{
		mRectangle.pos.x = Utility::randomInt(mContext.window->getSize().x - 25);
		mRectangle.pos.y = Utility::randomInt(mContext.window->getSize().y - 25);
		mTimeRemaining += mTimePerSquare;
	}

	double mx, my;
	unsigned mb;
	mContext.window->getMouseState(mx, my, mb);
	if ((mb & 1) != 0 && mRectangle.contains({mx, my}))
	{
		mPlayerScore++;
		mTimeRemaining = 0.f;
		mTimeWithoutHits = 0.f;
		if (mRectangle.size.x > MinSize)
		{
			mRectangle.size -= DeltaSize;
		}
		if (mTimePerSquare > MinTimePerSquare)
		{
			mTimePerSquare -= DeltaTime;
		}
	}
	else
	{
		mTimeWithoutHits += dt;
		mTimeRemaining -= dt;
		if (mTimeWithoutHits > MaxTimeWithoutHits)
		{
			mTimeWithoutHits -= MaxTimeWithoutHits;
			if (mRectangle.size.x < MaxSize)
			{
				mRectangle.size += DeltaSize;
			}
			if (mTimePerSquare < MaxTimePerSquare)
			{
				mTimePerSquare += DeltaTime;
			}
		}
	}
	mContext.window->setTitle("Score: " + std::to_string(mPlayerScore));

	return true;
}

bool
GameView::handleEvent(const Event &event)
{
	(void)event;
	return false;
}

void
GameView::render(RenderTarget &target)
{
	const Color color(0x99, 0xBB, 0xFF);
	target.clear(color);
	static const std::uint16_t indices[6] = { 0, 1, 2, 1, 3, 2 };
	static const glm::vec2 units[] = {
		{ 0.f, 0.f },
		{ 0.f, 1.f },
		{ 1.f, 0.f },
		{ 1.f, 1.f },
	};
	target.setTexture(&mContext.textures->get(TextureID::Square));
	auto base = target.getPrimIndex(6, 4);
	target.addIndices(base, indices + 0, indices + 6);
	auto vertices = target.getVertexArray(4);
	for (int i = 0; i < 4; i++)
	{
		vertices[i].pos = units[i] * mRectangle.size + mRectangle.pos;
		vertices[i].uv = units[i];
		vertices[i].color = colors[ mPlayerScore % 3 ];
	}
	target.draw();
}

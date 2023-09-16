#pragma once

#include "view.hpp"
#include "viewstack.hpp"
#include "rect.hpp"

class GameView: public View
{
public:
	GameView(ViewStack &stack, const Context &context);
	virtual ~GameView() override = default;

	virtual bool update(float dt) override;
	virtual bool handleEvent(const Event &event) override;
	virtual void render(RenderTarget &target) override;

private:
	ViewStack &mViewStack;
	const Context &mContext;
	int mPlayerScore;
	float mTimeRemaining;
	float mTimePerSquare;
	float mTimeWithoutHits;
	FloatRect mRectangle;
};

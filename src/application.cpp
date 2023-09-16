#include <cstdint>
#include <stdexcept>

#include <GLFW/glfw3.h>

#include "application.hpp"

#include "gameview.hpp"

namespace
{
const std::uint64_t targetFPS = 60;
const float SecondsPerFrame = 1.f / targetFPS;
const int MaxStepsPerFrame = 5;

const unsigned SCREEN_WIDTH = 800;
const unsigned SCREEN_HEIGHT = 600;
}

Application::Application()
	: mEventQueue()
	, mWindow()
	, mTarget()
	, mFonts()
	, mTextures()
	, mViewStack({ &mWindow, &mTarget, &mFonts, &mTextures, })
{
	if (!glfwInit())
	{
		const char *error;
		glfwGetError(&error);
		throw std::runtime_error(error);
	}

	registerViews();

	// push the first view
	mViewStack.pushView(ViewID::GamePlay);
	mViewStack.update(0.f);
}

Application::~Application()
{
	glfwTerminate();
}

void
Application::loadAssets()
{
	mTextures.load(TextureID::Square, "assets/textures/square.png");
}

void
Application::registerViews()
{
	mViewStack.registerView<GameView>(ViewID::GamePlay);
}

void
Application::run()
{
	mWindow.open("SquareChase", SCREEN_WIDTH, SCREEN_HEIGHT);

	// track the window events
	mEventQueue.track(mWindow);

	// tell the target to render on the window
	mTarget.use(mWindow);

	// with a context in use we load the assets
	loadAssets();

	// game loop
	const std::uint64_t deltaTicks = glfwGetTimerFrequency() / targetFPS;
	std::uint64_t currentTicks = glfwGetTimerValue();
	std::uint64_t accumulator = 0;
	while (!mWindow.isClosed() && !mViewStack.empty())
	{
		// update at constant frameTicks intervals
		auto newTicks = glfwGetTimerValue();
		accumulator += newTicks - currentTicks;
		currentTicks = newTicks;

		for (int steps = MaxStepsPerFrame;
		     steps > 0 && accumulator >= deltaTicks;
		     --steps, accumulator -= deltaTicks)
		{
			processInput();

			// update the view
			mViewStack.update(SecondsPerFrame);
		}

		// render
		mViewStack.render(mTarget);
		mWindow.display();
	}
}

void
Application::processInput()
{
	mEventQueue.poll();
	Event event;
	while (mEventQueue.pop(event))
	{
		if (mViewStack.handleEvent(event))
		{
			// event handled by a view in the stack
		}
		else if (const auto ep(std::get_if<KeyPressed>(&event)); ep
			 && ep->key == GLFW_KEY_ESCAPE)
		{
			mWindow.close();
		}
	}
}

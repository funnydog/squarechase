#pragma once

enum class ViewID
{
	None,
	Menu,
	Settings,
	GamePlay,
	GamePaused,
	GameOver,
};

enum class FontID
{
	Title,
	Subtitle,
	Body,
};

enum class TextureID
{
	Square,
};

template<typename Resource, typename Identifier>
class ResourceHolder;

class Font;
typedef ResourceHolder<Font, FontID> FontHolder;

class Texture;
typedef ResourceHolder<Texture, TextureID> TextureHolder;

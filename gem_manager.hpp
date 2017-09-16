#pragma once

#include <vector>
#include <tuple>

#include <SFML/Graphics.hpp>

#include "gem.hpp"

class GemManager
{
public:
	GemManager(int marginX, int marginY);
	~GemManager();
	
	void click(const sf::Vector2i& pos);
	void reset();
	void update();
	void draw(sf::RenderWindow& window);

	static constexpr int rows = 8;
	static constexpr int cols = 8;
	static constexpr int margin = 4;


private:
	bool match();
	// is it a vector needed here?
	std::vector<Gem> gems;
	sf::Texture texture;
	const int marginX;
	const int marginY;
	size_t selected;
	bool moving;
	bool arranging;

};
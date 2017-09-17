#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "gem.hpp"

using std::vector;

class GemManager
{
public:
	GemManager(float marginX, float marginY);
	~GemManager();
	
	void click(const sf::Vector2i& spos);
	void reset();
	void update();
	void draw(sf::RenderWindow& window);

	static constexpr int rows = 8;
	static constexpr int cols = 8;

private:
	bool match();
	// is it a vector needed here?
	vector<Gem>  gems;
	sf::Texture  texture;
	sf::Vector2f margin;
	size_t       selected;
	bool moving;
	bool arranging;

};
#pragma once

#include <SFML/Graphics.hpp>

class Gem
{
public:
	enum class Color : int
	{
		RED = 0, ORANGE = 1, YELLOW = 2, GREEN = 3, BLUE = 4, PURPLE = 5, WHITE = 6
	};

	Gem(int x, int y, int col, int row, Color color, sf::Texture& texture);
	~Gem();

	void draw(sf::RenderWindow& window);
	void changeAlpha(int delta);

private:
	int x, y;
	int col, row;
	sf::Sprite sprite;
	//, kind, match, alpha;

	Color color;

	enum class Status : int
	{
		NONE, SELECTED
	} status;

	int alpha;

};
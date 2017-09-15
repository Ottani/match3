#include <iostream>
using std::cout;

#include "gem.hpp"

Gem::Gem(int x, int y, int col, int row, Color color, sf::Texture &texture)
	: x(x), y(y), col(col), row(row), color(color), status(Status::NONE), alpha(255), sprite(texture)
{
	sprite.setTextureRect(sf::IntRect(static_cast<int>(color)*48, 0, 48, 48));
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	sprite.setPosition(x, y);
	//gems.move(offset.x-ts, offset.y-ts);
	
}


Gem::~Gem()
{

}

void Gem::draw(sf::RenderWindow& window)
{
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	window.draw(sprite);
}

void Gem::changeAlpha(int delta)
{
	alpha += delta;
	if (alpha < 0) alpha = 0;
	else if (alpha > 255) alpha = 255;
	cout << "new alpha: " << alpha << '\n';
}
	

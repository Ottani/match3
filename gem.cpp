#include <iostream>
using std::cout;

#include <SFML/Graphics.hpp>

#include "gem.hpp"

Gem::Gem(int x, int y, int col, int row, Color color, sf::Texture& texture)
	: x(x), y(y), tx(x), ty(y), col(col), row(row), color(color), status(Status::NONE), alpha(255), sprite(texture)
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
	sprite.setPosition(x, y);
	sprite.setTextureRect(sf::IntRect(static_cast<int>(color)*48, (status==Status::SELECTED)?48:0, 48, 48));
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	window.draw(sprite);
}

bool Gem::checkHit(int px, int py)
{
	if (status==Status::DELETED) return false;
	return (px >= x && px <= (x+size) && py >= y && py <= (y+size));
}

void Gem::swapTargets(Gem& other)
{
	std::swap(col, other.col);
	std::swap(row, other.row);
	
	tx = other.x;
	ty = other.y;
	
	other.tx = x;
	other.ty = y;
}

bool Gem::update()
{
	if (status==Status::DELETED) return false;
	if (status==Status::MATCH) {
		alpha -= 10;
		if (alpha <= 0) {
			alpha = 0;
			status = Status::DELETED;
			return false;
		}
		return true;
	}

	auto move = [=](int& p, int t) -> bool
	{
		if (p == t) return false;
		if (p < t) {
			p += std::min(5, t-p);
		} else {
			p -= std::min(5, p-t);
		}
		//status = Status::NONE;
		return true;
	};
	return move(x, tx) || move(y, ty);
}


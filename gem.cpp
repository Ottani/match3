#include <SFML/Graphics.hpp>
#include "gem.hpp"


const sf::Vector2f Gem::size {48.0f, 48.0f};

Gem::Gem(int col, int row, Color color, sf::Texture& texture, Status status)
	: col(col), row(row), pos(float((Gem::size.x+padding)*col), float((Gem::size.y+padding)*row)),
	target(pos), color(color), status(status), alpha(255), sprite(texture)
{
	
	sprite.setTextureRect(sf::IntRect(static_cast<int>(color)*48, 0, 48, 48));
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	sprite.setPosition(pos);
}

Gem::~Gem()
{

}

void Gem::draw(sf::RenderWindow& window, const sf::Vector2f& margin)
{
	if (status == Gem::Status::NEW) return;
	sprite.setPosition(pos + margin);
	sprite.setTextureRect(sf::IntRect(static_cast<int>(color)*48, (status==Status::SELECTED)?48:0, 48, 48));
	sprite.setColor(sf::Color(255, 255, 255, alpha));
	window.draw(sprite);
}

bool Gem::checkHit(const sf::Vector2f& spos)
{
	if (status==Status::DELETED) return false;
	//Rect (const Vector2< T > &position, const Vector2< T > &size)
	return sf::Rect<float>(pos, size).contains(spos);
	//return (px >= pos.x && px <= (pos.x+size) && py >= pos.y && py <= (pos.y+size));
}

void Gem::swapTargets(Gem& other)
{
	std::swap(col, other.col);
	std::swap(row, other.row);
	
	target = sf::Vector2f(float((Gem::size.x+padding)*col), float((Gem::size.y+padding)*row));
	other.target = sf::Vector2f(float((Gem::size.x+padding)*other.col), float((Gem::size.y+padding)*other.row));
}

bool Gem::update()
{
	if (status==Status::DELETED) return false;
	if (status==Status::MATCH) {
		alpha -= 10;
		if (alpha <= 50) {
			status = Status::DELETED;
			return false;
		}
		return true;
	}

	auto move = [=](float& p, float t) -> bool
	{
		if (p == t) return false;
		if (p < t) {
			p += std::min(5.0f, t-p);
		} else {
			p -= std::min(5.0f, p-t);
		}
		return true;
	};
	return move(pos.x, target.x) || move(pos.y, target.y);
}


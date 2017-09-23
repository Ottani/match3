#include <iostream>
#include <SFML/Graphics.hpp>
#include "gem.hpp"


const sf::Vector2f Gem::size {48.0f, 48.0f};

Gem::Gem(int col, int row, Color color, sf::Texture& texture, Status status, const sf::Font &font)
	: col(col), row(row), pos(float((Gem::size.x+padding)*col), float((Gem::size.y+padding)*row)),
	target(pos), color(color), status(status), alpha(255), sprite(texture), possibleMatch(false)
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
	if (status != Gem::Status::NEW) {
		sprite.setPosition(pos + margin);
		if (possibleMatch)
			sprite.setTextureRect(sf::IntRect(static_cast<int>(color)*48, 96, 48, 48));
		else
			sprite.setTextureRect(sf::IntRect(static_cast<int>(color)*48, (status==Status::SELECTED)?48:0, 48, 48));
		sprite.setColor(sf::Color(255, 255, 255, alpha));
		window.draw(sprite);
	}
}

bool Gem::checkHit(const sf::Vector2f& spos)
{
	if (status==Status::DELETED) return false;
	return sf::Rect<float>(pos, size).contains(spos);
}

void Gem::swapTargets(Gem& other)
{
	std::swap(col, other.col);
	std::swap(row, other.row);
	
	target = sf::Vector2f(float((Gem::size.x+padding)*col), float((Gem::size.y+padding)*row));
	other.target = sf::Vector2f(float((Gem::size.x+padding)*other.col), float((Gem::size.y+padding)*other.row));
	setStatus(Status::MOVING);
	other.setStatus(Status::MOVING);
}

Gem::Status Gem::update()
{
	switch(status)
	{
		case Status::MATCH:
		{
			setStatus(Status::DELETING);
			break;
		}
		case Status::DELETING:
		{
			alpha -= 10;
			if (alpha <= 50) {
				setStatus(Status::DELETED);
			}
			break;
		}
		case Status::MOVING:
		case Status::DELETED:
		{
			auto move = [=](float& p, float t) -> bool
			{
				if (p == t) return false;
				if (p < t) p += std::min(5.0f, t-p);
				else       p -= std::min(5.0f, p-t);
				return true;
			};
			if (!move(pos.x, target.x) && !move(pos.y, target.y) && status == Status::MOVING)
				setStatus(Status::NONE);
			break;
		}
		default:
			break;
	}
	return status;
}

void Gem::setStatus(Status status)
{
	if (this->status == Status::DELETED) return;
	this->status = status;
}

#include <iostream>
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

	if (!font.loadFromFile("images/sansation.ttf")) {
		std::cerr << "ERROR LOADING FONT\n";
	}
	text.setFont(font);
	text.setCharacterSize(40);
    text.setPosition(170.f, 150.f);
    text.setFillColor(sf::Color::White);
    text.setString("ola");
	//text.setCharacterSize(11);
	//text.setString("ola");
	//text.setColor(sf::Color::Red);
	//text.setStyle(sf::Text::Regular);
}

Gem::~Gem()
{

}

void Gem::draw(sf::RenderWindow& window, const sf::Vector2f& margin)
{
	if (status != Gem::Status::NEW) {
		sprite.setPosition(pos + margin);
		sprite.setTextureRect(sf::IntRect(static_cast<int>(color)*48, (status==Status::SELECTED)?48:0, 48, 48));
		sprite.setColor(sf::Color(255, 255, 255, alpha));
		window.draw(sprite);
	}
	//text.setPosition(pos + margin);
	/*switch(status) {
		case Status::NEW:      text.setString("NEW"); break;
		case Status::NONE:     text.setString("NONE"); break;
		case Status::SELECTED: text.setString("SLCTED"); break;
		case Status::MATCH:    text.setString("MATCH"); break;
		case Status::MOVING:   text.setString("MOVE"); break;
		case Status::DELETING: text.setString("DELETING"); break;
		case Status::DELETED:  text.setString("DELETED"); break;
		default: text.setString("??"); break;
	}
	
	*/
	window.draw(text);
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
	status = other.status = Status::MOVING;
}

Gem::Status Gem::update()
{
	switch(status)
	{
		case Status::MATCH:
		{
			status = Status::DELETING;
			break;
		}
		case Status::DELETING:
		{
			alpha -= 10;
			if (alpha <= 50) {
				status = Status::DELETED;
			}
			break;
		}
		case Status::MOVING:
		{
			auto move = [=](float& p, float t) -> bool
			{
				if (p == t) return false;
				if (p < t) p += std::min(5.0f, t-p);
				else       p -= std::min(5.0f, p-t);
				return true;
			};
			if (!move(pos.x, target.x) && !move(pos.y, target.y))
				status = Status::NONE;
			break;
		}
		default:
			break;
	}
	return status;
}

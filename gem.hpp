#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

class Gem
{
public:
	enum class Color : int
	{
		RED = 0, ORANGE = 1, YELLOW = 2, GREEN = 3, BLUE = 4, PURPLE = 5, WHITE = 6
	};

	enum class Status : int
	{
		NONE, SELECTED, MATCH, DELETED
	};

	Gem(int x, int y, int col, int row, Color color, sf::Texture& texture);
	~Gem();

	void draw(sf::RenderWindow& window);
	bool checkHit(int px, int py);
	void swapTargets(Gem& other);
	bool update();

	int getCol() const { return col; }
	int getRow() const { return row; }
	Color getColor() const { return color; }
	Status getStatus() const { return status; }
	void setStatus(Status status) 
	{
		if (this->status == Status::DELETED) {
			std::cout << "OPA\n";
		}
		this->status = status;
	}

	static constexpr int size = 48;

private:
	int x, y, tx, ty, col, row;
	sf::Sprite sprite;
	Color      color;
	Status     status;
	int        alpha;

};


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

	const Gem* findGemAtXY(int x, int y);
	const Gem* findGem(int col, int row);
	//const Gem* findGem(int col, int row);
	std::tuple<int, int> getXY(int col, int row);
	
	void click(const sf::Vector2i& pos);
	void reset();
	void draw(sf::RenderWindow& window);

	static constexpr int rows = 8;
	static constexpr int cols = 8;
	static constexpr int margin = 4;
	static constexpr int size = 48;


private:
	std::vector<Gem> gems;
	sf::Texture texture;
	const int marginX;
	const int marginY;

};
#include <iostream>
using std::cout;

#include "gem_manager.hpp"


GemManager::GemManager(int marginX, int marginY) : marginX(marginX), marginY(marginY)
{
	// TODO use new c++ rng
	srand(time(0));
	texture.loadFromFile("images/gems.png");
}

GemManager::~GemManager()
{

}

void GemManager::reset()
{
	gems.clear();
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			gems.emplace_back(marginX + (size+margin)*c, marginY + (size+margin)*r, c, r, static_cast<Gem::Color>(rand()%7), texture);
		}
	}
}

void GemManager::draw(sf::RenderWindow& window)
{
	for (auto& g : gems) {
		g.draw(window);
	}
}

void GemManager::click(const sf::Vector2i& pos)
{
	int c = (pos.x - marginX) / (size+margin);
	int r = (pos.y - marginY) / (size+margin);
	cout << "pos.x = " << pos.x << ", x = " << c
		<< "\npos.y = " << pos.y << ", y = " << r << '\n';
	if (c >= 0 && c < cols && r >= 0 && r < rows) {
		gems[(r * cols) + c].changeAlpha(-25);
	}
}

const Gem* GemManager::findGemAtXY(int x, int y)
{
	return nullptr;

}

const Gem* GemManager::findGem(int col, int row)
{
	return nullptr;
}

std::tuple<int, int> GemManager::getXY(int col, int row)
{
	return std::make_tuple<int, int>((size+margin)*col, (size+margin)*row);
}

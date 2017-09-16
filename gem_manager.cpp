#include <limits>
#include <algorithm>
#include <iostream>

using std::get;
using std::cout;

#include "gem_manager.hpp"


GemManager::GemManager(int marginX, int marginY) : marginX(marginX), marginY(marginY), moving(false), arranging(false)
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
			gems.emplace_back(marginX + (Gem::size+margin)*c, marginY + (Gem::size+margin)*r, c, r, static_cast<Gem::Color>(rand()%7), texture);
		}
	}
	selected = std::numeric_limits<size_t>::max();
	moving = false;
	arranging = false;
}

void GemManager::draw(sf::RenderWindow& window)
{
	for (auto& g : gems) {
		g.draw(window);
	}
}

void GemManager::click(const sf::Vector2i& pos)
{
	if (moving || arranging) return;
	for (auto& gem : gems) {
		if (gem.checkHit(pos.x, pos.y)) {
			if (selected != std::numeric_limits<size_t>::max()) {
				auto& other = gems[selected];
				if ((abs(gem.getCol()-other.getCol()) + abs(gem.getRow()-other.getRow())) != 1) break;
				std::iter_swap(&gem, &other);
				other.swapTargets(gem);
				gem.setStatus(Gem::Status::NONE);
				other.setStatus(Gem::Status::NONE);
				selected = std::numeric_limits<size_t>::max();
			} else {
				selected = gem.getRow() * cols + gem.getCol();
				gem.setStatus(Gem::Status::SELECTED);
				cout << "selected " << selected << '\n';
			}
			break;
		}
	}
}

void GemManager::update()
{
	moving = false;
	for (auto& gem : gems) {
		if (gem.update()) moving = true;
	}
	if(moving) return;

	if (!arranging)	match();
	arranging = false;
	for(auto gem = gems.rbegin(); gem != gems.rend(); ++gem) {
		if (gem->getStatus() == Gem::Status::DELETED) {
			if (gem->getRow() > 0) {
				arranging = true;
				if ((gem + cols)->getStatus() != Gem::Status::MATCH) {
					std::iter_swap(gem, gem + cols);
					gem->swapTargets(*(gem + cols));
					break;
				}
			} else {
				int c = gem->getCol();
				int r = gem->getRow();
				*gem = Gem(marginX + (Gem::size+margin)*c, marginY + (Gem::size+margin)*r, c, r, static_cast<Gem::Color>(rand()%7), texture);
			}
		}
	}
	
}

bool GemManager::match()
{
	bool match = false;
	auto gem = gems.begin();
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			if (gem->getStatus() == Gem::Status::DELETED) continue;
			if (c > 0 && c < (cols-1) && (gem+1)->getColor()==gem->getColor() && (gem-1)->getColor()==gem->getColor()) {
				(gem-1)->setStatus(Gem::Status::MATCH);
				(gem)->setStatus(Gem::Status::MATCH);
				(gem+1)->setStatus(Gem::Status::MATCH);
				match = true;
			}

			if (r > 0 && r < rows-1 && (gem+cols)->getColor()==gem->getColor() && (gem-cols)->getColor()==gem->getColor()) {
				(gem-cols)->setStatus(Gem::Status::MATCH);
				(gem)->setStatus(Gem::Status::MATCH);
				(gem+cols)->setStatus(Gem::Status::MATCH);
				match = true;
			}
			++gem;
		}
	}
	return match;
}

#include <limits>
#include <algorithm>
#include "gem_manager.hpp"


GemManager::GemManager(float marginX, float marginY) : margin(marginX, marginY), moving(false), arranging(false)
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
			gems.emplace_back(c, r, static_cast<Gem::Color>(rand()%7), texture, Gem::Status::NONE);
		}
	}
	selected = std::numeric_limits<size_t>::max();
	moving = false;
	arranging = false;
}

void GemManager::draw(sf::RenderWindow& window)
{
	for (auto& g : gems) {
		g.draw(window, margin);
	}
}

void GemManager::click(const sf::Vector2i& spos)
{
	if (moving || arranging) return;
	const sf::Vector2f pos {spos.x - margin.x, spos.y - margin.y};
	for (auto& gem : gems) {
		if (gem.checkHit(pos)) {
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
	if(!moving) {
		for (auto& gem : gems) {
			if (gem.getStatus() == Gem::Status::NEW) gem.setStatus(Gem::Status::NONE);
		}
	
		if (!arranging)	match();
	}

	arranging = false;
	for(auto gem = gems.begin(); gem != gems.end(); ++gem) {
		if (gem->getStatus() == Gem::Status::DELETED) {
			if (gem->getRow() > 0) {
				arranging = true;
				std::iter_swap(gem, gem - cols);
				gem->swapTargets(*(gem - cols));
			} else {
				int c = gem->getCol();
				int r = gem->getRow();
				*gem = Gem(c, r, static_cast<Gem::Color>(rand()%7), texture, Gem::Status::NEW);
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

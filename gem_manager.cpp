#include <iostream>
#include <algorithm>
#include "gem_manager.hpp"


GemManager::GemManager(float marginX, float marginY) : margin(marginX, marginY), state(State::WAITING)
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
	setState(State::WAITING);
}

void GemManager::draw(sf::RenderWindow& window)
{
	for (auto& g : gems) {
		g.draw(window, margin);
	}
}

void GemManager::click(const sf::Vector2i& spos)
{
	if (state != State::WAITING && state != State::FIRST_SEL) return;
	const sf::Vector2f pos {spos.x - margin.x, spos.y - margin.y};
	for (auto& gem : gems) {
		if (gem.checkHit(pos)) {
			if (state == State::FIRST_SEL) {
				sel2 = gem.getRow() * cols + gem.getCol();
				auto& other = gems[sel1];
				if ((abs(gem.getCol()-other.getCol()) + abs(gem.getRow()-other.getRow())) != 1) break;
				std::iter_swap(&gem, &other);
				other.swapTargets(gem);
				gem.setStatus(Gem::Status::NONE);
				other.setStatus(Gem::Status::NONE);
				setState(State::SECOND_SEL);
			} else {
				sel1 = gem.getRow() * cols + gem.getCol();
				gem.setStatus(Gem::Status::SELECTED);
				setState(State::FIRST_SEL);
			}
			break;
		}
	}
}

void GemManager::update()
{
	if (state == State::WAITING || state == State::SECOND_SEL) {
		if (match()) {
			setState(State::MOVING);
		} else if (state == State::SECOND_SEL) {
			auto& gem = gems[sel1];
			auto& other = gems[sel2];
			std::iter_swap(&gem, &other);
			other.swapTargets(gem);
			gem.setStatus(Gem::Status::NONE);
			other.setStatus(Gem::Status::NONE);
			setState(State::MOVING);
		}
	}

	if (state == State::MOVING) {
		bool moving = false;
		for (auto& gem : gems) {
			if (gem.update()) moving = true;
		}
		if(!moving) {
			for (auto& gem : gems) {
				if (gem.getStatus() == Gem::Status::NEW) gem.setStatus(Gem::Status::NONE);
			}
			setState(State::ARRANGING);
		}
	}

	if (state == State::ARRANGING) {
		bool arranging = false;
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
		if (!arranging) {
			setState(State::WAITING);
		} else {
			setState(State::MOVING);
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

void GemManager::setState(State newState)
{
	std::cout << "Changing state from " << static_cast<int>(state) << " to " << static_cast<int>(newState) << '\n';
	state = newState;
}

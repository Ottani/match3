#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include "gem_manager.hpp"


GemManager::GemManager(float marginX, float marginY) : margin(marginX, marginY), state(State::WAITING), font(),
		score(0), scoreText("Score: 000000", font, 20)
{
	// TODO use new c++ rng
	srand(time(0));
	if (!texture.loadFromFile("images/gems.png")) {
		std::cerr << "ERROR LOADING IMAGE\n";
		exit(1);
	}
	if (!font.loadFromFile("images/sansation.ttf")) {
		std::cerr << "ERROR LOADING FONT\n";
		exit(2);
	}
	scoreText.setFillColor(sf::Color::Black);
}

GemManager::~GemManager()
{

}

void GemManager::reset()
{
	gems.clear();
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			gems.emplace_back(c, r, static_cast<Gem::Color>(rand()%7), texture, Gem::Status::NONE, font);
		}
	}
	score = 0;
	setState(State::WAITING);
}

void GemManager::draw(sf::RenderWindow& window)
{
	for (auto& g : gems) {
		g.draw(window, margin);
	}
	std::ostringstream sstr;
	sstr << "Score: " << std::setw(6) << std::setfill('0') << score;
	scoreText.setString(sstr.str());
	scoreText.setPosition(580, 10);
	window.draw(scoreText);
}

void GemManager::click(const sf::Vector2i& spos)
{
	if (state != State::WAITING && state != State::SELECTED) return;
	const sf::Vector2f pos {spos.x - margin.x, spos.y - margin.y};
	for (auto& gem : gems) {
		if (gem.checkHit(pos)) {
			if (state == State::SELECTED) {
				clearPossibleMatch();
				sel2 = gem.getRow() * cols + gem.getCol();
				auto& other = gems[sel1];
				if ((abs(gem.getCol()-other.getCol()) + abs(gem.getRow()-other.getRow())) != 1) break;
				std::iter_swap(&gem, &other);
				other.swapTargets(gem);
				setState(State::SWAPPING);
			} else {
				sel1 = gem.getRow() * cols + gem.getCol();
				gem.setStatus(Gem::Status::SELECTED);
				setState(State::SELECTED);
			}
			break;
		}
	}
}

void GemManager::update()
{
	if (state == State::WAITING) {
		for (auto& gem : gems) {
			if (gem.getStatus() == Gem::Status::NEW) gem.setStatus(Gem::Status::NONE);
		}
		int m = match();
		if (m > 0) {
			setState(State::MOVING);
			score += m;
		} else if (arrange()) {
			setState(State::MOVING);
		}
	}

	if (state == State::MOVING || state == State::SWAPPING) {
		bool moving = false;
		for (auto& gem : gems) {
			Gem::Status status = gem.update();
			if (status == Gem::Status::MOVING || status == Gem::Status::DELETING) moving = true;
		}
		if (!moving) {
			if (state == State::SWAPPING) {
				int m = match();
				score += m;
				if (m == 0) {
					auto& gem = gems[sel1];
					auto& other = gems[sel2];
					std::iter_swap(&gem, &other);
					other.swapTargets(gem);
				}
				setState(State::MOVING);
			} else {
				setState(State::WAITING);
			}
		}
	}
}

int GemManager::match()
{
	int match = 0;
	auto gem = gems.begin();
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c, ++gem) {
			if (c > 0 && c < (cols-1)) {
				match += match3(&*gem , &*(gem+1), &*(gem-1));
			}
			if (r > 0 && r < rows-1) {
				match += match3(&*gem , &*(gem+cols), &*(gem-cols));
			}
		}
	}
	return match;
}

int GemManager::match3(Gem* gem1, Gem* gem2, Gem* gem3)
{
	if (gem1->getStatus() != Gem::Status::NONE && gem1->getStatus() != Gem::Status::MATCH
			&& gem2->getStatus() != Gem::Status::NONE && gem2->getStatus() != Gem::Status::MATCH
			&& gem3->getStatus() != Gem::Status::NONE && gem3->getStatus() != Gem::Status::MATCH) {
		return 0;
	}
	int match = 0;
	if (gem1->getColor() == gem2->getColor() && gem1->getColor() == gem3->getColor()) {
		if (gem1->getStatus() != Gem::Status::MATCH) {
			++match;
			gem1->setStatus(Gem::Status::MATCH);
		}
		if (gem2->getStatus() != Gem::Status::MATCH) {
			++match;
			gem2->setStatus(Gem::Status::MATCH);
		}
		if (gem3->getStatus() != Gem::Status::MATCH) {
			++match;
			gem3->setStatus(Gem::Status::MATCH);
		}
	}
	return match;
}

bool GemManager::arrange()
{
	bool arranging = false;
	for(auto gem = gems.rbegin(); gem != gems.rend(); ++gem) {
		if (gem->getStatus() != Gem::Status::DELETED) continue;
		if (gem->getRow() == 0) break;
		arranging = true;
		for (int row = 1; row <= gem->getRow(); ++row) {
			if ((gem+(cols*row))->getStatus() != Gem::Status::DELETED) {
				std::iter_swap(gem, (gem+(cols*row)));
				gem->swapTargets(*(gem+(cols*row)));
				break;
			}
		}
	}
	replaceDeleted();
	return arranging;
}

void GemManager::replaceDeleted()
{
	for(auto gem = gems.begin(); gem != gems.end(); ++gem) {
		if (gem->getStatus() == Gem::Status::DELETED) {
			*gem = Gem(gem->getCol(), gem->getRow(), static_cast<Gem::Color>(rand()%7), texture, Gem::Status::NEW, font);
		}
	}
}

void GemManager::setState(State newState)
{
	state = newState;
}

bool GemManager::findPossibleMatch(bool show)
{
	if (state != State::WAITING) return false;
	clearPossibleMatch();
	for (int r = rows-1; r >= 0; --r) {
		for (int c = 0; c < cols; ++c) {
			Gem* gem = &gems[r*cols + c];
			Gem::Color color = gem->getColor();
			//   3
			// 1 x 2
			//   3
			if (c < cols-2 && (gem+2)->getColor() == color) {
				if (r > 0 && checkMatch3(gem, gem+2, gem+1-cols, show)) return true;
				if (r < rows-1 && checkMatch3(gem, gem+2, gem+1+cols, show)) return true;
			}
			//   1
			// 3 x 3
			//   2
			if (r < rows-2 && (gem+2*cols)->getColor() == color) {
				if (c > 0 && checkMatch3(gem, gem+2*cols, gem-1+cols, show)) return true;
				if (c < cols-1 && checkMatch3(gem, gem+2*cols, gem+1+cols, show)) return true;
			}
			//     3
			// 1 2 x 3
			//     3
			if (c < cols-2 && (gem+1)->getColor() == color) {
				if (r > 0 && checkMatch3(gem, gem+1, gem+2-cols, show)) return true;
				if (r < rows-1 && checkMatch3(gem, gem+1, gem+2+cols, show)) return true;
				if (c < cols-3 && checkMatch3(gem, gem+1, gem+3, show)) return true;
			}
			//   3
			// 3 x 2 1
			//   3
			if (c > 1 && (gem-1)->getColor() == color) {
				if (r > 0 && checkMatch3(gem, gem-1, gem-2-cols, show)) return true;
				if (r < rows-1 && checkMatch3(gem, gem-1, gem-2+cols, show)) return true;
				if (c > 2 && checkMatch3(gem, gem-1, gem-3, show)) return true;
			}
			//   1
			//   2
			// 3 x 3
			//   3
			if (r < rows-2 && (gem+cols)->getColor() == color) {
				if (c > 0 && checkMatch3(gem, gem+cols, gem-1+2*cols, show)) return true;
				if (c < cols-1 && checkMatch3(gem, gem+cols, gem+1+2*cols, show)) return true;
				if (r < rows-3 && checkMatch3(gem, gem+cols, gem+3*cols, show)) return true;
			}
			//   3
			// 3 x 3
			//   2
			//   1
			if (r > 1 && (gem-cols)->getColor() == color) {
				if (c > 0 && checkMatch3(gem, gem-cols, gem-1-2*cols, show)) return true;
				if (c < cols-1 && checkMatch3(gem, gem-cols, gem+1-2*cols, show)) return true;
				if (r > 2 && checkMatch3(gem, gem-cols, gem-3*cols, show)) return true;
			}
		}
	}
	return false;
}

bool GemManager::checkMatch3(Gem* gem1, Gem* gem2, Gem* gem3, bool show)
{
	//if (gem1->getColor() == gem2->getColor() && gem1->getColor() ==  gem3->getColor()) {
	// check only gem1 and 3 as gem2 was already matched with gem1
	if (gem1->getColor() == gem3->getColor()) {
		if (show) {
			gem1->setPossibleMatch(true);
			gem2->setPossibleMatch(true);
			gem3->setPossibleMatch(true);
		}
		return true;
	}
	return false;
}

void GemManager::clearPossibleMatch()
{
	for (auto &gem : gems)
		gem.setPossibleMatch(false);

}


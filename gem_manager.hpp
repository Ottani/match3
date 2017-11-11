#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "gem.hpp"

using std::vector;

class GemManager
{
public:
	GemManager() = default;
	~GemManager() = default;
	
	bool init();
	void click(const sf::Vector2f& spos);
	void reset();
	void update();
	void draw(sf::RenderWindow& window);
	bool findPossibleMatch(bool show);
	bool spawnBomb();
	bool isLosing();

	static constexpr int rows = 8;
	static constexpr int cols = 8;
	static constexpr int pointsPerBomb = 50;
	static constexpr int pointsPerTip = 50;

	int getScore() const { return score; }

private:
	// State or Status?
	enum class State : int
	{
		WAITING, SELECTED, SWAPPING, MOVING, ARRANGING, LOSING
	};

	void setState(State newState);
	int match();
	int match3(Gem* gem1, Gem* gem2, Gem* gem3);
	bool arrange();
	void replaceDeleted();
	bool checkMatch3(Gem* gem1, Gem* gem2, Gem* gem3, bool show);
	void clearPossibleMatch();
	void explode(Gem* gem);

	vector<Gem>  gems;
	sf::Texture  texture;
	size_t       sel1;
	size_t       sel2;
	State        state = State::WAITING;
	int          score = 0;
	int          latestBomb;
	bool	     usedTip;
};

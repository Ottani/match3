#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "gem.hpp"

using std::vector;

class GemManager
{
public:
	GemManager(float marginX, float marginY);
	~GemManager();
	
	void click(const sf::Vector2i& spos);
	void reset();
	void update();
	void draw(sf::RenderWindow& window);

	static constexpr int rows = 8;
	static constexpr int cols = 8;

private:
	// State or Status?
	enum class State : int
	{
		WAITING, SELECTED, SWAPPING, MOVING, ARRANGING
	};

	void setState(State newState);
	int match();
	int match3(Gem* gem1, Gem* gem2, Gem* gem3);
	bool arrange();
	void replaceDeleted();

	vector<Gem>  gems;
	sf::Texture  texture;
	sf::Vector2f margin;
	sf::Font     font;
	sf::Text     scoreText;
	size_t       sel1;
	size_t       sel2;
	State        state;
	int          score;

};

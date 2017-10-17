#include <iostream>
#include <iomanip>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "gem_manager.hpp"

using namespace sf;

class Match
{
public:
	Match() :font(), scoreText("Score: 000000", font, 20), losingText("Game Over!", font, 80)
	{
		scoreText.setFillColor(Color::Black);
		losingText.setFillColor(Color::Black);
	}
	~Match() {}

	bool init(unsigned int width, unsigned int height)
	{
		window.create(VideoMode(width, height), "Match-3");
		window.setFramerateLimit(60);
		// this is only for maintaing the text score in place.
		screenView.reset(FloatRect(0, 0, width, height));
		screenView.setViewport(FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
		
		if (!texBackground.loadFromFile("resources/background.jpg")) {
			std::cerr << "ERROR LOADING BACKGROUND IMAGE\n";
			return false;
		}
		if (!font.loadFromFile("resources/sansation.ttf")) {
			std::cerr << "ERROR LOADING FONT\n";
			return false;
		}
		background.setTexture(texBackground);
		background.setScale((float)width/texBackground.getSize().x, (float)height/texBackground.getSize().y);

		double edge = (64.0f*8)+(4*7);
		gemView.reset(FloatRect(0, 0, edge, edge));
		gemView.setViewport(FloatRect(0.028125f, 0.05f, 0.50625f, 0.9f));
		window.setView(gemView);
		gemManager.init();
	}

	void run()
	{
		while(window.isOpen()) {
			Event e;
			while(window.pollEvent(e)) {
				switch(e.type) {
					case Event::Closed:
						window.close();
						break;
					case Event::Resized:
					{
						screenView.reset(FloatRect(0.0f, 0.0f, e.size.width, e.size.height));
						background.setScale((float)e.size.width/texBackground.getSize().x, (float)e.size.height/texBackground.getSize().y);
						float aspect = (float)e.size.height / (float)e.size.width;
						gemView.setViewport(FloatRect(0.05f * aspect, 0.05f, 0.9f * aspect, 0.9f));
						break;
					}
					case Event::KeyReleased:
						if (e.key.code == Keyboard::Escape) window.close();
						if (e.key.code == Keyboard::Q) gemManager.findPossibleMatch(true);
						if (e.key.code == Keyboard::W) gemManager.spawnBomb();
						break;
					case Event::MouseButtonPressed:
						if (e.mouseButton.button == Mouse::Left) {
							gemManager.click(window.mapPixelToCoords(Mouse::getPosition(window)));
						}
						break;
					default:
						break;
				}
			}
	
			gemManager.update();
			draw();
	
			
		}
	}


private:
	void draw()
	{
		window.setView(screenView);
		window.draw(background);

		std::ostringstream sstr;
		sstr << "Score: " << std::setw(6) << std::setfill('0') << gemManager.getScore();
		scoreText.setString(sstr.str());
		scoreText.setPosition(window.getSize().x - 200, 25);
		losingText.setPosition(window.getSize().x / 2, window.getSize().y / 2);

		window.draw(scoreText);

		if (gemManager.isLosing()) {
			window.draw(losingText);
		}

		window.setView(gemView);
		gemManager.draw(window);
		
		window.display();
	}


	RenderWindow window;
	View         gemView;
	View         screenView;
	Texture      texBackground;
	Sprite       background;
	Font         font;
	Text         scoreText;
	Text         losingText;
	GemManager   gemManager;
};


int main()
{
	Match app;
	app.init(960, 540);
	app.run();
		
	return 0;
}

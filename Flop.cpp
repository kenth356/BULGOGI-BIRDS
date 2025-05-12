// Mariano, Kenth Jarren S. 
// Bulgogi Birds (A Flappy Birds Clone)
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

// CONSTANT VARIABLES FOR INGAME ESSENTIALS
const float IG_gravity = 0.5f;
const float IG_flap_strength = -10.f;
const float IG_pipe_speed = 5.f;
const float IG_pipe_width = 80.f;
const float IG_pipe_gap = 200.f;
const float IG_spawn_intv = 2.0f;

// GAME VARIABLES
enum GameVARS { GV_menu, GV_skinselect, GV_game, GV_pause, GV_finish };
GameVARS gamevars = GV_menu;

// GAME BACKGROUND VIA IMAGE/SPRITES/TEXTURE
Texture background_TEXT;
Sprite background_SPRT;

// FOR BIRDS SKINS
vector<Texture> bird_SKINS(3);
int currSKIN_INDEX = 0;

// FOR FONTS
Font font;

// FOR CURRENT SCORING/GAMEOVER FUNCTION/PIPE SPEED HANDLING
int score = 0;
bool gameOVER = false;
float pipeSPEED = IG_pipe_speed;

// FOR BACKGROUND AUDIO (FADED BY ALAN WALKER)
Music backgroundAUDIO;

// FOR PIPE TEXTURES (TOP AND BOTTOM)
Texture pipeTEXT_TOP, pipeTEXT_BOT;

// IN GAME BIRD CLASS
class BIRD {
public:
	Sprite bird_SPRT;
	float velocity;

	BIRD() {
		velocity = 0;
	}

	void setTEXTURE(const Texture& texture) {
		bird_SPRT.setTexture(texture);
		bird_SPRT.setScale(0.3f, 0.3f);
		bird_SPRT.setPosition(100, 600 / 2);
	}

	void birdFLAP() {
		velocity = IG_flap_strength;
	}

	void updateBIRD() {
		velocity += IG_gravity;
		bird_SPRT.move(0, velocity);
	}

	FloatRect getBounds() const {
		return bird_SPRT.getGlobalBounds();
	}
};

// IN GAME PIPES CLASS
class PIPES {
public:
	RectangleShape TOP, BOTTOM;
	float verticalOFFSET;
	bool moveUP;

	PIPES(float x, float topHEIGHT, const Texture& textTOP, const Texture& textBOT) {
		verticalOFFSET = 0.f;
		moveUP = rand() % 2;
		
		TOP.setSize(Vector2f(IG_pipe_width, topHEIGHT));
		TOP.setPosition(x, 0);
		TOP.setTexture(&textTOP);

		BOTTOM.setSize(Vector2f(IG_pipe_width, 800 - topHEIGHT - IG_pipe_gap));
		BOTTOM.setPosition(x, topHEIGHT + IG_pipe_gap);
		BOTTOM.setTexture(&textBOT);
	}

	void updateFLAP() {
		TOP.move(-pipeSPEED, 0);
		BOTTOM.move(-pipeSPEED, 0);

		if (score >= 25) {
			float moveAMOUNT = 1.5f;
			float moveRANGE = 50.f;

			if (moveUP) {
				verticalOFFSET += -moveAMOUNT;
			}
			else {
				verticalOFFSET += moveAMOUNT;
			}

			if (abs(verticalOFFSET) >= moveRANGE) {
				moveUP = !moveUP;
			}

			if (moveUP) {
				TOP.move(0, -moveAMOUNT);
				BOTTOM.move(0, -moveAMOUNT);
			}
			else {
				TOP.move(0, moveAMOUNT);
				BOTTOM.move(0, moveAMOUNT);
			}
		}
	}

	bool screenOFF() const {
		return TOP.getPosition().x + IG_pipe_width < 0;
	}

	FloatRect getTopBounds() const {
		return TOP.getGlobalBounds();
	}

	FloatRect getBottomBounds() const {
		return BOTTOM.getGlobalBounds();
	}
};

int main() {
	RenderWindow window(VideoMode(800, 600), "Bulgogi Birds");
	window.setFramerateLimit(60);
	Event ev;

	// FOR LOADING THE FONT
	font.loadFromFile("C:\\Users\\samon\\source\\repos\\Flappy Bird Clone By Mariano, Kenth\\ASSETS\\PressStart2P-Regular.ttf");

	// FOR LOADING THE BACKGROUND IMAGE
	background_TEXT.loadFromFile("C:\\Users\\samon\\source\\repos\\Flappy Bird Clone By Mariano, Kenth\\ASSETS\\BACKGROUND.png");

	// FOR SETTING THE TEXTURE ONTO THE SPRITES
	background_SPRT.setTexture(background_TEXT);
	background_SPRT.setScale(
		800.f / background_TEXT.getSize().x,
		600.f / background_TEXT.getSize().y
	);

	// FOR LOADING THE SKINS FOR BIRDS VIA IMAGE/SPRITES/TEXTURES
	bird_SKINS[0].loadFromFile("C:\\Users\\samon\\source\\repos\\Flappy Bird Clone By Mariano, Kenth\\ASSETS\\BIRD.png");
	bird_SKINS[1].loadFromFile("C:\\Users\\samon\\source\\repos\\Flappy Bird Clone By Mariano, Kenth\\ASSETS\\BIRD2.png");
	bird_SKINS[2].loadFromFile("C:\\Users\\samon\\source\\repos\\Flappy Bird Clone By Mariano, Kenth\\ASSETS\\BIRD3.png");

	// FOR LOADING THE PIPES' SPRITES/TEXTURES
	pipeTEXT_TOP.loadFromFile("C:\\Users\\samon\\source\\repos\\Flappy Bird Clone By Mariano, Kenth\\ASSETS\\SPRITETOP.png");
	pipeTEXT_BOT.loadFromFile("C:\\Users\\samon\\source\\repos\\Flappy Bird Clone By Mariano, Kenth\\ASSETS\\SPRITEBOT.png");

	// FOR LOADING THE BACKGROUND AUDIO (FADED BY ALAN WALKER)
	backgroundAUDIO.openFromFile("C:\\Users\\samon\\source\\repos\\Flappy Bird Clone By Mariano, Kenth\\ASSETS\\faded.mp3");
	backgroundAUDIO.setLoop(true);
	backgroundAUDIO.play();

	BIRD birdload;
	birdload.setTEXTURE(bird_SKINS[currSKIN_INDEX]);

	// FOR IN GAME TEXTS (STRINGS) MANAGING
	Text scoreTEXT, menuTEXT, skinselectTEXT, gameoverTEXT, pauseTEXT, finishTEXT;
	// FOR SCORES
	scoreTEXT.setFont(font);
	scoreTEXT.setCharacterSize(40);
	scoreTEXT.setFillColor(Color::White);
	scoreTEXT.setOutlineColor(Color::Black);
	scoreTEXT.setOutlineThickness(5);
	scoreTEXT.setPosition(10, 10);
	// FOR MENU
	menuTEXT.setFont(font);
	menuTEXT.setCharacterSize(40);
	menuTEXT.setFillColor(Color::White);
	menuTEXT.setOutlineColor(Color::Black);
	menuTEXT.setOutlineThickness(5);
	menuTEXT.setString("1. START\n\n2. SKINS\n\n3. EXIT");
	menuTEXT.setPosition(238, 220);
	// FOR SKIN SELECTION
	skinselectTEXT.setFont(font);
	skinselectTEXT.setCharacterSize(40);
	skinselectTEXT.setFillColor(Color::White);
	skinselectTEXT.setOutlineColor(Color::Black);
	skinselectTEXT.setOutlineThickness(5);
	skinselectTEXT.setString("SKIN SELECTION\n\n\n  1. Classic\n\n  2. Epic\n\n  3. Legend");
	skinselectTEXT.setPosition(140, 130);
	// FOR GAMEOVER TEXT
	gameoverTEXT.setFont(font);
	gameoverTEXT.setCharacterSize(40);
	gameoverTEXT.setFillColor(Color::White);
	gameoverTEXT.setOutlineColor(Color::Black);
	gameoverTEXT.setOutlineThickness(5);
	gameoverTEXT.setString("GAME OVER \n\n    ;(");
	gameoverTEXT.setPosition(225, 250);
	// FOR PAUSE TEXT
	pauseTEXT.setFont(font);
	pauseTEXT.setCharacterSize(40);
	pauseTEXT.setFillColor(Color::White);
	pauseTEXT.setOutlineColor(Color::Black);
	pauseTEXT.setOutlineThickness(5);
	pauseTEXT.setString("\n  GAME\n\n PAUSED");
	pauseTEXT.setPosition(235, 225);
	// FOR 1000TH SCORE TEXT
	finishTEXT.setFont(font);
	finishTEXT.setCharacterSize(40);
	finishTEXT.setFillColor(Color::White);
	finishTEXT.setOutlineColor(Color::Black);
	finishTEXT.setOutlineThickness(5);
	finishTEXT.setString("\n GGS :D");
	finishTEXT.setPosition(225, 250);
	// FOR MENU-UI-BOX
	RectangleShape menuBOX(Vector2f(400, 250));
	menuBOX.setFillColor(Color(255, 255, 255, 200));
	menuBOX.setOutlineColor(Color::Black);
	menuBOX.setOutlineThickness(3);
	menuBOX.setPosition(200, 200);
	// FOR PAUSE-UI-BOX
	RectangleShape pauseBOX(Vector2f(400, 250));
	pauseBOX.setFillColor(Color(255, 255, 255, 200));
	pauseBOX.setOutlineColor(Color::Black);
	pauseBOX.setOutlineThickness(3);
	pauseBOX.setPosition(200, 200);
	// FOR 1000TH-UI-BOX
	RectangleShape finishBOX(Vector2f(400, 250));
	finishBOX.setFillColor(Color(255, 255, 255, 200));
	finishBOX.setOutlineColor(Color::Black);
	finishBOX.setOutlineThickness(3);
	finishBOX.setPosition(200, 200);
	// FOR IN GAME MECHANICS AND KEYS
	Clock clock;
	float timeLASTSPAWN = 0;
	vector<PIPES> pipes;

	// IN GAME LOOP!!!
	while (window.isOpen()) {
		while (window.pollEvent(ev)) {
			switch (ev.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:	
				if (gamevars == GV_menu) {
					if (ev.key.code == Keyboard::Num1)
						gamevars = GV_game;
					if (ev.key.code == Keyboard::Num2)
						gamevars = GV_skinselect;
					if (ev.key.code == Keyboard::Num3)
						window.close();
				}
				else if (gamevars == GV_skinselect) {
					if (ev.key.code == Keyboard::Num1)
						currSKIN_INDEX = 0;
					if (ev.key.code == Keyboard::Num2)
						currSKIN_INDEX = 1;
					if (ev.key.code == Keyboard::Num3)
						currSKIN_INDEX = 2;
					birdload.setTEXTURE(bird_SKINS[currSKIN_INDEX]);
					gamevars = GV_menu;
				}
				else if (gamevars == GV_game) {
					if (!gameOVER && ev.key.code == Keyboard::Space)
						birdload.birdFLAP();
					if (!gameOVER && ev.key.code == Keyboard::Escape)
						gamevars = GV_pause;
					if (gameOVER && ev.key.code == Keyboard::R) {
						birdload.bird_SPRT.setPosition(100, 600 / 2);
						birdload.velocity = 0.f;
						pipes.clear();
						score = 0;
						timeLASTSPAWN = 0.f;
						gameOVER = false;
						pipeSPEED = IG_pipe_speed;
						clock.restart();
					} if (gameOVER && ev.key.code == Keyboard::M) {
						gamevars = GV_menu;
						birdload.bird_SPRT.setPosition(100, 600 / 2);
						birdload.velocity = 0.f;
						pipes.clear();
						score = 0;
						timeLASTSPAWN = 0.f;
						gameOVER = false;
						pipeSPEED = IG_pipe_speed;
						clock.restart();
					}
				}
				else if (gamevars == GV_pause) {
					if (ev.key.code == Keyboard::Escape) {
						gamevars = GV_game;
						clock.restart();
					} if (ev.key.code == Keyboard::M) {
						gamevars = GV_menu;
						birdload.bird_SPRT.setPosition(100, 600 / 2);
						birdload.velocity = 0.f;
						pipes.clear();
						score = 0;
						timeLASTSPAWN = 0.f;
						gameOVER = false;
						pipeSPEED = IG_pipe_speed;
						clock.restart();
					}
				}
				else if (gamevars == GV_finish) {
					if (ev.key.code == Keyboard::M) {
						gamevars = GV_menu;
						birdload.bird_SPRT.setPosition(100, 600 / 2);
						birdload.velocity = 0.f;
						pipes.clear();
						score = 0;
						timeLASTSPAWN = 0.f;
						gameOVER = false;
						pipeSPEED = IG_pipe_speed;
						clock.restart();
					}
				}
				break;
			}
		}
		window.clear();

		if (gamevars == GV_menu) {
			window.draw(background_SPRT);
			window.draw(menuBOX);
			window.draw(menuTEXT);
		}
		else if (gamevars == GV_skinselect) {
			window.draw(background_SPRT);
			window.draw(skinselectTEXT);
		}
		else if (gamevars == GV_game) {
			float TIME = clock.restart().asSeconds();
			timeLASTSPAWN += TIME;
			if (!gameOVER) {
				birdload.updateBIRD();
				if (timeLASTSPAWN >= IG_spawn_intv) {
					float maxTOP = 600 - IG_pipe_gap - 50;
					float heightTOP = static_cast<float>(rand() % static_cast<int>(maxTOP - 50) + 50);
					pipes.emplace_back(800, heightTOP, pipeTEXT_TOP, pipeTEXT_BOT);
					timeLASTSPAWN = 0.f;
				}

				for (size_t i = 0; i < pipes.size(); ++i) {
					pipes[i].updateFLAP();
					if (pipes[i].screenOFF()) {
						pipes.erase(pipes.begin() + i);
						score++;
						if (score >= 1000) {
							gamevars = GV_finish;
							break;
						}
							--i;
					}
				}

				for (const auto& pipe : pipes) {
					if (birdload.getBounds().intersects(pipe.getTopBounds()) || birdload.getBounds().intersects(pipe.getBottomBounds()))
						gameOVER = true;
				}

				if (birdload.bird_SPRT.getPosition().y < 0 || birdload.bird_SPRT.getPosition().y + birdload.bird_SPRT.getGlobalBounds().height > 600)
					gameOVER = true;
				if (score >= 20)
					pipeSPEED = 8.f;
				if (score >= 35)
					pipeSPEED = 10.f;
				if (score >= 50)
					pipeSPEED = 12.f;
				if (score >= 75)
					pipeSPEED = 15.f;
				if (score >= 1000)
					gamevars = GV_finish;
			}

			scoreTEXT.setString("Score: " + to_string(score));
			window.draw(background_SPRT);
			window.draw(birdload.bird_SPRT);
			for (auto& pipe2 : pipes) {
				window.draw(pipe2.TOP);
				window.draw(pipe2.BOTTOM);
			}

			window.draw(scoreTEXT);
			if (gameOVER)
				window.draw(gameoverTEXT);
		}
		else if (gamevars == GV_pause) {
			window.draw(background_SPRT);
			window.draw(pauseBOX);
			window.draw(pauseTEXT);
		}
		else if (gamevars == GV_finish) {
			window.draw(background_SPRT);
			window.draw(finishBOX);
			window.draw(finishTEXT);
		}

		window.display();
		
	}

	return 0;

}

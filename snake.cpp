#include <iostream>
#include <vector>
#include <queue>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <atomic>
#include <thread>
#include <random>
#include <algorithm>
#include <string>
#include <unordered_map>
#include "controls.hpp"
enum class Direction {
	RIGHT = 0,
	DOWN = 1,
	LEFT = 2,
	UP = 3
};

class Game {
private:
	unsigned int width, height, score;
	float blockSize, scoreBoardW;
	bool paused, isGameOver;
	std::unordered_map<std::string, sf::Color> colors;
	std::vector<std::pair<int, int>> snake;
	Direction dir;
	sf::RenderWindow& window;
	sf::Font font;
	sf::Time speed;
	sf::Clock clock;
	sf::Texture apple;
	sf::Sprite appleSprite;
	sf::Texture head;
	sf::Sprite headSprite;
	sf::Music foodFx;
	sf::Music gameOverFx;
	sf::Music moveFx;
	std::mt19937 rng;
	std::uniform_int_distribution<int> range1, range2;
	std::pair<int, int> food;
	std::queue<std::pair<int, int>> moves;
	controls::Button startBtn, newGameBtn;
	controls::Label scoreLabel, pointsLabel, gameOverLabel;

public:
	Game(int w, int h, float bs, sf::RenderWindow& window) :window(window), rng(std::random_device{}()) {//bs = blockSize
		//load assets
		font.loadFromFile("assets/font/font.ttf");
		apple.loadFromFile("assets/sprites/food.png");
		head.loadFromFile("assets/sprites/head.png");
		foodFx.openFromFile("assets/audiofx/eat.wav");
		moveFx.openFromFile("assets/audiofx/move.wav");
		gameOverFx.openFromFile("assets/audiofx/gameover.wav");
		//game configuration
		colors = { {"BLACK", sf::Color(0, 0, 0)}, {"GREY", sf::Color(12, 12, 12)}, {"GREEN", sf::Color(51, 255, 51)}, {"RED", sf::Color(255, 0, 0)}, {"CONTROL", sf::Color(50, 50, 50)}, {"WHITE", sf::Color(200, 200, 200)}, {"DARKGREEN", sf::Color(204, 219, 56)} };
		blockSize = bs;
		width = w / blockSize;
		height = h / blockSize;
		scoreBoardW = 160 / blockSize;
		range1 = std::uniform_int_distribution<int>(0, width - scoreBoardW - 1);
		range2 = std::uniform_int_distribution<int>(0, height - 1);
		//game controls
		newGameBtn.create(&window, scoreBoardW * blockSize, 100, "New Game");
		startBtn.create(&window, scoreBoardW * blockSize, 100, "Start");
		scoreLabel.create(&window, "SCORE");
		gameOverLabel.create(&window, "GAME OVER!");
		pointsLabel.create(&window, "0");
		apple.setSmooth(true);
		appleSprite.setTexture(apple);
		appleSprite.setScale(0.03f, 0.03f);
		sf::FloatRect lb = appleSprite.getLocalBounds();
		appleSprite.setOrigin(lb.left + lb.width / 2.0f, lb.top + lb.height / 2.0f);
		head.setSmooth(true);
		headSprite.setTexture(head);
		headSprite.setScale(0.067f, 0.067f);
		lb = headSprite.getLocalBounds();
		headSprite.setOrigin(lb.left + lb.width / 2.0f, lb.top + lb.height / 2.0f);
	}
	void newGame() {
		//Reset everything
		isGameOver = false;
		paused = true;
		dir = Direction::RIGHT;
		while (!moves.empty())
			moves.pop();
		moves.push({ 1, 0 });
		score = 0;
		speed = sf::seconds(0.2f);
		snake.clear();
		snake = { {2, 0}, {1, 0}, {0, 0} };
		startBtn.setText("Start");
		pointsLabel.create(&window, "0");
		getFood();
	}

	void render() {
		window.clear(colors["CONTROL"]);
		//render board
		drawBorad();
		// render snake
		drawSnake();
		//render food
		appleSprite.setPosition(food.first * blockSize + blockSize / 2.0f, food.second * blockSize + blockSize / 2.0f);
		window.draw(appleSprite);
		//render controls
		drawControl();
		//render score
		drawScore();
		if (isGameOver)
			drawGameOver();
		window.display();
	}
	void drawBorad() {
		sf::RectangleShape square(sf::Vector2f(blockSize, blockSize));
		for (int i = 0; i < width - scoreBoardW; i++) {
			for (int j = 0; j < height; j++) {
				if ((i + j) & 1 == 1)
					square.setFillColor(colors["BLACK"]);
				else
					square.setFillColor(colors["GREY"]);
				square.setPosition(i * blockSize, j * blockSize);
				window.draw(square);
			}
		}
	}
	void drawSnake() {
		sf::RectangleShape square(sf::Vector2f(blockSize, blockSize));
		square.setOutlineThickness(-blockSize / 10);
		square.setOutlineColor(colors["DARKGREEN"]);
		for (int i = 0; i < snake.size(); i++) {
			square.setFillColor(colors["DARKGREEN"]);
			square.setPosition(snake[i].first * blockSize, snake[i].second * blockSize);
			window.draw(square);
			if (i == 0) {
				switch (moves.front().first) {
				case 1:
					headSprite.setRotation(270.0f);
					headSprite.setPosition(snake[i].first * blockSize + blockSize / 2.0f + 10, snake[i].second * blockSize + blockSize / 2.0f);
					break;
				case -1:
					headSprite.setRotation(90.0f);
					headSprite.setPosition(snake[i].first * blockSize + blockSize / 2.0f - 10, snake[i].second * blockSize + blockSize / 2.0f);
					break;
				}
				switch (moves.front().second) {
				case -1:
					headSprite.setRotation(180.0f);
					headSprite.setPosition(snake[i].first * blockSize + blockSize / 2.0f, snake[i].second * blockSize + blockSize / 2.0f - 10);
					break;
				case 1:
					headSprite.setRotation(360.0f);
					headSprite.setPosition(snake[i].first * blockSize + blockSize / 2.0f, snake[i].second * blockSize + blockSize / 2.0f + 10);
					break;
				}
				window.draw(headSprite);
			}


		}
	}
	void drawScore() {
		scoreLabel.setPos((width - scoreBoardW + scoreBoardW / 2.0f) * blockSize, 50);//center as origin
		scoreLabel.setColor(colors["WHITE"]);
		scoreLabel.setFontSize(30);
		scoreLabel.draw();
		pointsLabel.setText(std::to_string(score));
		//relative to score label and center as origin
		pointsLabel.setPos(scoreLabel.getPos().left + scoreLabel.getPos().width / 2, scoreLabel.getPos().top + scoreLabel.getPos().height + 50);
		pointsLabel.setColor(colors["GREEN"]);
		pointsLabel.setFontSize(50);
		pointsLabel.draw();
	}
	void drawControl() {
		//relative to pointsLabel and center as origin
		newGameBtn.setPos(pointsLabel.getPos().left + pointsLabel.getPos().width / 2, pointsLabel.getPos().top + pointsLabel.getPos().height + 50);
		newGameBtn.draw();
		//relative to newGameBtn and center as origin
		startBtn.setPos(newGameBtn.getPos().left + newGameBtn.getPos().width / 2, newGameBtn.getPos().top + newGameBtn.getPos().height);
		startBtn.draw();
	}
	void drawGameOver() {
		sf::RectangleShape square(sf::Vector2f((width - scoreBoardW) * blockSize, height * blockSize));
		square.setFillColor(sf::Color(0, 0, 0, 150));
		window.draw(square);
		gameOverLabel.setPos((width - scoreBoardW) * blockSize / 2, height * blockSize / 2);//center as origin
		gameOverLabel.setColor(colors["WHITE"]);
		gameOverLabel.setFontSize(100);
		gameOverLabel.draw();

	}
	void move() {
		if (clock.getElapsedTime() > speed) {
			//snake head & tail
			auto head = snake.front();
			auto tail = snake.back();
			//update head position
			auto m = moves.front();
			//Move soundfx
			 if(m.first == 1 && dir != Direction::RIGHT){
			     dir = Direction::RIGHT;
			     moveFx.play();
			 }
			 if(m.first == -1 && dir != Direction::LEFT){
			     dir = Direction::LEFT;
			     moveFx.play();
			 }
			 if(m.second == 1 && dir != Direction::DOWN){
			     dir = Direction::DOWN;
			     moveFx.play();
			 }
			 if(m.second == -1 && dir != Direction::UP){
			     dir = Direction::UP;
			     moveFx.play();
			 }
			head.first += m.first;
			head.second += m.second;
			//Minimum one move required to keep snake mmoving
			if (moves.size() > 1)
				moves.pop();
			//cheack for collision
			if (isCollided(head.first, head.second)) {
				isGameOver = true;
				gameOverFx.play();
				paused = true;
			}
			//update snake block by snake[i] takes snake[i + 1] psoition
			if (!paused) {
				for (auto i = snake.rbegin(); i != snake.rend() - 1; i++)
					*i = *(i + 1);
				*snake.begin() = head;
			}
			//check food collision
			if (head == food) {
				drawScore();
				snake.push_back(tail);
				score += 5;
				if (!(score % 20))
					speed -= sf::seconds(0.01f);
				foodFx.play();
				getFood();
			}
			clock.restart();
		}
	}
	void mouseClick() {
		if (newGameBtn.isClicked()) {
			newGame();
		}
		else if (!isGameOver && startBtn.isClicked()) {
			if (paused) {
				paused = false;
				startBtn.setText("Pause");
			}
			else {
				paused = true;
				startBtn.setText("Start");
			}
		}

	}

	void updateDir(Direction d) {
		if (!paused) {
			int x = 0, y = 0;
			switch (d) {
			case Direction::LEFT:
				x = -1;
				break;
			case Direction::RIGHT:
				x = 1;
				break;
			case Direction::DOWN:
				y = 1;
				break;
			case Direction::UP:
				y = -1;
				break;
			};
			auto m = moves.back();
			if (m.first + x != 0 || m.second + y != 0 && m != std::make_pair(x, y)) {
				moves.push({ x, y });
			}
		}
	}
	void getFood() {
		int x = range1(rng);
		int y = range2(rng);
		while (std::find(snake.begin(), snake.end(), std::make_pair(x, y)) != snake.end()) {
			x = range1(rng);
			y = range2(rng);
		}
		food = { x, y };
	}
	bool isCollided(int x, int y) {
		if (x < 0 || x >= width - scoreBoardW || y < 0 || y >= height || std::find(snake.begin(), snake.end(), std::make_pair(x, y)) != snake.end())
			return true;
		return false;
	}
	void run() {
		render();
		if (!paused) {
			move();
		}
	}
};

int main() {
	const int width = 1000, height = 800;
	const float blockSize = 40.0f;//dont change!
	sf::RenderWindow window(sf::VideoMode(width, height), "Snake Game", sf::Style::Close);
	Game game(width, height, blockSize, window);
	game.newGame();
	window.setKeyRepeatEnabled(false);
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case 73:
					game.updateDir(Direction::UP);
					break;
				case 72:
					game.updateDir(Direction::RIGHT);
					break;
				case 74:
					game.updateDir(Direction::DOWN);
					break;
				case 71:
					game.updateDir(Direction::LEFT);
					break;
				}
				break;
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left)
					game.mouseClick();
			}
		}
		game.run();
	}

	return 0;
}
#include "controls.hpp"
namespace controls {
	Label::Label() {
		font.loadFromFile("assets/font/font.ttf");
		color = sf::Color(204, 229, 255);
		fontSize = 20;
	};
	void Label::create(sf::RenderWindow* window, std::string text) {
		this->window = window;
		this->label = text;
	}
	void Label::draw() {
		text.setFont(font);
		text.setString(label);
		text.setCharacterSize(fontSize);
		text.setFillColor(color);
		sf::FloatRect textBound = text.getLocalBounds();
		text.setOrigin(textBound.left + textBound.width / 2.0f, textBound.top + textBound.height / 2.0f);
		text.setPosition(x, y);
		window->draw(text);
		pos = text.getGlobalBounds();
	}
	void Label::setText(std::string s) {
		this->label = s;
	}
	void Label::setColor(sf::Color color) {
		this->color = color;
	}
	void Label::setPos(float x, float y) {
		this->x = x;
		this->y = y;
	}
	void Label::setFontSize(int size) {
		fontSize = size;
	}
	sf::FloatRect Label::getPos() {
		return pos;
	}

	Button::Button() {
		font.loadFromFile("assets/font/font.ttf");
		color = sf::Color(204, 229, 255);
		colorHover = sf::Color::Red;
	};
	void  Button::create(sf::RenderWindow* window, float width, float height, std::string text) {
		this->window = window;
		this->width = width;
		this->height = height;
		this->label = text;
	}
	void  Button::draw() {
		button = sf::RectangleShape(sf::Vector2f(width, height));
		sf::FloatRect boxBound = button.getLocalBounds();
		button.setOrigin(boxBound.left + boxBound.width / 2.0f, boxBound.top + boxBound.height / 2.0f);
		button.setFillColor(sf::Color::Transparent);
		button.setPosition(x, y);
		text.setFont(font);
		text.setString(label);
		text.setCharacterSize(20);
		text.setFillColor(isHover() ? colorHover : color);
		boxBound = button.getGlobalBounds();
		sf::FloatRect textBound = text.getLocalBounds();
		text.setOrigin(textBound.left + textBound.width / 2.0f, textBound.top + textBound.height / 2.0f);
		text.setPosition(boxBound.left + boxBound.width / 2.0f, boxBound.top + boxBound.height / 2.0f);
		window->draw(button);
		window->draw(text);
		pos = boxBound;
	}
	bool  Button::isClicked() {
		sf::Vector2f mouse = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
		return text.getGlobalBounds().contains(mouse) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
	}
	bool  Button::isHover() {
		sf::Vector2f mouse = static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
		return text.getGlobalBounds().contains(mouse);
	}
	void  Button::setText(std::string s) {
		this->label = s;
	}
	void  Button::setColor(sf::Color color) {
		this->color = color;
	}
	void  Button::setPos(float x, float y) {
		this->x = x;
		this->y = y;
	}
	sf::FloatRect  Button::getPos() {
		return pos;
	}

}
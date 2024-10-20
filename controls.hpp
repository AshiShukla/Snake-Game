#ifndef CONTROLS
#define CONTROLS

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace controls {
    class Label {
    private:
        float x, y;
        std::string label;
        sf::RenderWindow* window;
        sf::Font font;
        sf::Text text;
        sf::Color color;
        sf::FloatRect pos;
        int fontSize;
    public:
        Label();
        void create(sf::RenderWindow* window, std::string text);
        void draw();
        void setText(std::string s);
        void setColor(sf::Color color);
        void setPos(float x, float y);
        void setFontSize(int size);
        sf::FloatRect getPos();

    };
    class Button {
    private:
        float width, height, x, y;
        std::string label;
        sf::RenderWindow* window;
        sf::Font font;
        sf::RectangleShape button;
        sf::Text text;
        sf::Color color, colorHover;
        sf::FloatRect pos;
    public:
        Button();
        void create(sf::RenderWindow* window, float width, float height, std::string text);
        void draw();
        bool isClicked();
        bool isHover();
        void setText(std::string s);
        void setColor(sf::Color color);
        void setPos(float x, float y);
        sf::FloatRect getPos();

    };
}

#endif
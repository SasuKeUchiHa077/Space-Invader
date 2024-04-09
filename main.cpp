#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>

#define WIDTH 1280
#define HEIGHT 720

int main (int argc, char *argv[]) {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Invaders", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(360);

    //Structure : SHIP ------------------------------------
    sf::Texture shipTexture;
    if (!shipTexture.loadFromFile("Assets/ship.png")) {
        return EXIT_FAILURE;
    }

    sf::RectangleShape ship;
    ship.setSize(sf::Vector2f(125.0f, 125.0f));
    ship.setOrigin(sf::Vector2f(ship.getSize().x / 2.0f, ship.getSize().y / 2.0f));
    ship.setTexture(&shipTexture);
    ship.setPosition(sf::Vector2f(100.0f, HEIGHT / 2.0f));
    ship.rotate(90.0f);

    //Animation for ship boosterr (The fire thing behind the rocket).-----------------
    sf::Texture boosterTexture;
    if (!boosterTexture.loadFromFile("Assets/boosterAnimation.png")) {
        return EXIT_FAILURE;
    }

    float boosterTextureRectWidth = 48.0f;
    float boosterTextureRectheight = 48.0f;

    sf::IntRect boosterTextureRect(0, 0, boosterTextureRectWidth, boosterTextureRectheight);

    sf::RectangleShape booster;
    booster.setSize(sf::Vector2f(75.0f, 75.0f));
    booster.setOrigin(sf::Vector2f(booster.getSize().x / 2.0f, booster.getSize().y / 2.0f));
    booster.setTexture(&boosterTexture);
    booster.setTextureRect(boosterTextureRect);
    booster.rotate(90.0f);

    //Rocket Structure.-------------------------------------------------------
    sf::Texture rocketTexture;
    if (!rocketTexture.loadFromFile("Assets/rocket.png")) {
        return EXIT_FAILURE;
    }

    float rocketTextureRectWidth = 32.0f;
    float rocketTextureRectHeight = 32.0f;

    sf::IntRect rocketTextureRect(0, 0, rocketTextureRectWidth, rocketTextureRectHeight);

    std::vector<sf::RectangleShape> Rockets; //vector to store multiple rockets.

    //Alien.------------------------------------------------------------------
    sf::Texture alienTexture;
    if (!alienTexture.loadFromFile("Assets/alien.png")) {
        return EXIT_FAILURE;
    }

    sf::RectangleShape alien;
    alien.setSize(sf::Vector2f(125.0f, 125.0f));
    alien.setOrigin(sf::Vector2f(alien.getSize().x / 2.0f, alien.getSize().y / 2.0f));
    alien.setTexture(&alienTexture);
    alien.setPosition(sf::Vector2f(WIDTH - alien.getSize().x - 30, HEIGHT / 2.0f));

    //other stuffs.---------
    sf::Clock clock1;
    float rocketShootDelay = 0.3f;

    sf::Clock clock2;
    float rocketAnimationDelay = 0.07f;

    sf::Clock clock3;
    float boosterAnimationDelay = 0.07f;

    //handeling window --------------------------------------
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }

        //Updating ship position (Movement) --------------------------------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            ship.move(0.0f, -4.0f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            ship.move(-4.0f, 0.0f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            ship.move(0.0f, 4.0f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            ship.move(4.0f, 0.0f);
        }

        //Boundary limit for the ship ---------------------------------------
        sf::Vector2f shipPosition = ship.getPosition();

        if (shipPosition.x < ship.getOrigin().x / 2.0f) {
            ship.setPosition(ship.getOrigin().x / 2.0f, shipPosition.y);
        }

        if (shipPosition.x > WIDTH - ship.getOrigin().x / 2.0f) {
            ship.setPosition(WIDTH - ship.getOrigin().x / 2.0f, shipPosition.y);
        }

        if (shipPosition.y < ship.getOrigin().y / 2.0f) {
            ship.setPosition(shipPosition.x, ship.getOrigin().y / 2.0f);
        }

        if (shipPosition.y > HEIGHT - ship.getOrigin().y / 2.0f) {
            ship.setPosition(shipPosition.x, HEIGHT - ship.getOrigin().y / 2.0f);
        }

        //Firing rockets function(Not the actual function).-------------------
        if (clock1.getElapsedTime().asSeconds() >= rocketShootDelay) { 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                sf::RectangleShape rocket;
                rocket.setSize(sf::Vector2f(96.0f, 96.0f));
                rocket.setOrigin(sf::Vector2f(rocket.getSize().x / 2.0f, rocket.getSize().y / 2.0f));
                rocket.setTexture(&rocketTexture);
                rocket.setTextureRect(rocketTextureRect);
                rocket.rotate(90.0f);
                rocket.setPosition(sf::Vector2f(ship.getPosition().x + 30.0f, ship.getPosition().y));

                Rockets.push_back(rocket);
                clock1.restart();
            }
        }

        //animation for the rocket
        if (clock2.getElapsedTime().asSeconds() > rocketAnimationDelay) {
            rocketTextureRect.left += rocketTextureRectWidth;
            if (rocketTextureRect.left >= rocketTextureRectWidth * 3) {
                rocketTextureRect.left = 0;
            }
            clock2.restart();
        }

        //animation for the Ship booster
        if (clock3.getElapsedTime().asSeconds() > boosterAnimationDelay) {
            boosterTextureRect.left += boosterTextureRectWidth;
            if (boosterTextureRect.left >= boosterTextureRectWidth * 4) {
                boosterTextureRect.left = 0;
            }
            booster.setTextureRect(boosterTextureRect);
            clock3.restart();
        }

        booster.setPosition(sf::Vector2f(ship.getPosition())); //setting position of the booster as same as the ship.


        //windowrendering stuff ----------------------------------------------
        window.clear();

        window.draw(alien);

        //displaying and updating the rockets from the vector.----------------
        for (auto &rocket : Rockets) {
            rocket.setTextureRect(rocketTextureRect);

            rocket.move(8.0f, 0.0f);
            window.draw(rocket);

            Rockets.erase(std::remove_if(Rockets.begin(), Rockets.end(), [&](const sf::RectangleShape &rocket) {return rocket.getPosition().x > WIDTH;}), Rockets.end());
        }

        window.draw(ship);

        window.draw(booster);

        window.display();
    }
    return 0;
}

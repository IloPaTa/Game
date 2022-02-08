#include "Menu.h"

#include "Game.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "AnimationManager.h"
#include "Button.h"
#include "Player.h"
#include <Windows.h>

Menu::Menu(int w, int h) {
    W = w;
    H = h;
    int border = 50;
    int btn_x = W / 2 - W / 12;
    int btn_w = 360, btn_h = 115;

    score = L"0";

    font.loadFromFile("fonts/sunday.ttf");
    startGame.setImage("images/menuButtons.png", 12, 158);
    startGame.setText(L"Играть");
    startGame.setCharacterSize(30);
    startGame.setPosition(btn_x, H/2);
    startGame.setSize(btn_w, btn_h);
   

    settings.setImage("images/menuButtons.png", 12, 158);
    settings.setText(L"Настройки");
    settings.setCharacterSize(30);
    settings.setPosition(btn_x, H/2 + W/28 + border);
    settings.setSize(btn_w, btn_h);

    exit.setImage("images/menuButtons.png", 12, 158);
    exit.setText(L"Выход");
    exit.setCharacterSize(30);
    exit.setPosition(btn_x, H / 2 + W / 14 + 2* border);
    exit.setSize(btn_w, btn_h);
    
    title.setFont(font);
    title.setString(L"Слизь в слизистой");
    title.setPosition(sf::Vector2f(W/3.25, H/6));
    title.setFillColor(sf::Color::Black);
    title.setCharacterSize(80);

    record.setFont(font);
    record.setString(L"Рекорд:" + score);
    record.setPosition(sf::Vector2f(W / 2.75, H / 3));
    record.setFillColor(sf::Color::Black);
    record.setCharacterSize(60);
}

void Menu::run() {
    setlocale(LC_ALL, "Russian");
    sf::RenderWindow window(sf::VideoMode(W, H), "Prc", sf::Style::Fullscreen);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                W = static_cast<double>(event.size.width);
                H = static_cast<double>(event.size.height);
                sf::Vector2f windowSize = sf::Vector2f(event.size.width, event.size.height);
                window.setView(sf::View(sf::Vector2f(windowSize.x / 2,
                    windowSize.y / 2), sf::Vector2f(windowSize)));
            }
            
            if (event.type == sf::Event::MouseButtonReleased) {
                if (startGame.isPressed(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                    startGame.setImage("images/menuButtons.png", 12, 158);
                    Game game(W, H);
                    score  = game.run(window);
                }
                if (exit.isPressed(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                    exit.setImage("images/menuButtons.png", 12, 158);
                    window.close();
                }

            }
        }

        if (startGame.isPressed(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
            startGame.setImage("images/menuButtons.png", 382, 158);
        }
        else {
            startGame.setImage("images/menuButtons.png", 12, 158);
        }

        if (exit.isPressed(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
            exit.setImage("images/menuButtons.png", 382, 158);
        }
        else {
            exit.setImage("images/menuButtons.png", 12, 158);
        }

        if (settings.isPressed(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
            settings.setImage("images/menuButtons.png", 382, 158);
        }
        else {
            settings.setImage("images/menuButtons.png", 12, 158);
        }
        record.setString(L"Рекорд:" + score);
        window.clear(sf::Color(99, 0, 199));
        startGame.draw(window);
        settings.draw(window);
        exit.draw(window);
        window.draw(title);
        window.draw(record);
        window.display();
    }


    //freopen("logRecord/record.txt", "w", stdin);
    
    return;
}
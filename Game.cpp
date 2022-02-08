#include "Game.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "AnimationManager.h"
#include "Button.h"
#include "Enemy.h"
#include "Level.h"
#include "Player.h"

#include <Windows.h>

std::wstring Game::to_string(int x) {
    std::wstring a;
    while (x != 0) {
        a += x % 10 + '0';
        x /= 10;
    }
    reverse(a.begin(), a.end());
    return a;
}

std::wstring Game::run(sf::RenderWindow& window) {
    setlocale(LC_ALL, "Russian");
    sf::Texture q;
    q.loadFromFile("images/characters.png");

    Level lvl;
    lvl.LoadFromFile("mP.tmx");

    Button exit(localW - 70, 10, 50, 50);
    exit.setText(L"X");
    exit.setCharacterSize(20);

    AnimationManager anim;
    anim.create("walk", q, 256, 160, 47, 80, 3, 0.005, 80);
    anim.create("stay", q, 336, 0, 47, 80, 1, 0.005, 80);
    anim.create("up", q, 256, 240, 47, 80, 3, 0.005, 80);
    anim.create("down", q, 256, 0, 47, 80, 3, 0.005, 80);


    AnimationManager enemy_anim;
    enemy_anim.create("walk", q, 0, 480, 80, 80, 3, 0.005, 80);
    enemy_anim.create("stay", q, 0, 320, 80, 80, 1, 0.005, 80);
    enemy_anim.create("up", q, 0, 560, 80, 80, 3, 0.005, 80);
    enemy_anim.create("down", q, 0, 320, 80, 80, 3, 0.005, 80);

    Player hero(anim);
    hero.x = localW / 2;
    hero.y = localH / 2;

    std::vector<Object> first[3];
    first[0] = lvl.GetObjects("enemy1");
    first[1] = lvl.GetObjects("enemy2");

    AnimationManager bullet_anim;
    sf::Texture bullet_q;
    bullet_q.loadFromFile("images/bullet.png");
    bullet_anim.create("shoot", bullet_q ,0, 0, 47, 47, 1, 0.003, 0);
    hero.setBulletAnim(bullet_anim);

    AnimationManager slime_anim;
    sf::Texture slime_q;
    slime_q.loadFromFile("images/slime_bullet.png");
    slime_anim.create("shoot", slime_q, 0, 0, 47, 47, 1, 0.003, 0);

    sf::Text score;
    sf::Font font;
    font.loadFromFile("fonts/sunday.ttf");
    score.setFont(font);
    score.setString(L"Score: 0");
    score.setFillColor(sf::Color::Green);
    score.setCharacterSize(50);
    score.setPosition(1200, 10);

    sf::Clock clock;

    int cnt = 0;
    int delay_shoot = 0; 
    int waves = -1;
    int scr = 0;
    while (window.isOpen())
    {
        if (enemies.size() == 0) {
            ++waves;
            waves %= 2;
            hero.setHp(20);
            for (int i = 0; i < first[waves].size(); ++i) {
                Enemy enemy(enemy_anim, first[waves][i].rect.left, first[waves][i].rect.top);
                enemy.setBoost();
                enemy.setBulletAnim(slime_anim);
                enemies.push_back(enemy);
            }
        }
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        time = time / 400; 

        if (time > 40) time = 40;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                localW = static_cast<double>(event.size.width);
                localH = static_cast<double>(event.size.height);
                sf::Vector2f windowSize = sf::Vector2f(event.size.width, event.size.height);
                window.setView(sf::View(sf::Vector2f(windowSize.x / 2,
                    windowSize.y / 2), sf::Vector2f(windowSize)));
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (exit.isPressed(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                    return to_string(scr);
                }
            }

        }
        anim.set("stay");
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            hero.key["R"] = 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            hero.key["L"] = 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            hero.key["D"] = 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            hero.key["U"] = 1;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (abs(cnt - delay_shoot) > 500) {
                delay_shoot = cnt;
                hero.shoot(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
            }
        }
        std::vector<Object> walls = lvl.GetObjects("wall");
        std::vector<Object> checkpoint = lvl.GetObjects("checkpoint");

        hero.update(time, walls, enemies);
        if (cnt == 1000) {
            for (int i = 0; i < enemies.size(); ++i) {
                enemies[i].setBoost();
            }
        }
        if (hero.isCollision(lvl.GetObjects("checkpoint"))) {
            return to_string(scr);
        }
        for (int i = 0; i < enemies.size(); ++i) {
            if (enemies[i].isDead()) {
                enemies.erase(enemies.begin() + i);
                --i;
                ++scr;
                continue;
            }
            int damage = enemies[i].update(time, walls, hero.getRect());
            hero.getDamage(damage);
        }
        if (hero.isDead()) {
            return to_string(scr);
        }

        std::wstring tmp = L"Score: " + to_string(scr);
        score.setString(tmp);
        window.clear(sf::Color(184, 160, 175));
        lvl.Draw(window);
        hero.draw(window);
        for (int i = 0; i < enemies.size(); ++i) {
            enemies[i].draw(window);
        }
        exit.draw(window);
        window.draw(score);
        window.display();
        ++cnt;
    }
    return to_string(scr);
}

Game::Game(int W, int H){
    localW = W;
    localH = H;
}

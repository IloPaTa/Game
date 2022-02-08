#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"
class Game {
public:
    int localW, localH;
    std::wstring run(sf::RenderWindow &window);
    Game(int localW, int localH);
private:
    std::vector<Enemy> enemies;
    std::wstring to_string(int x);
};


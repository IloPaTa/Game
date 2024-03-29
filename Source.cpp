#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Game.h"
#include "Menu.h"
#include <Windows.h>

int W = GetSystemMetrics(SM_CXSCREEN);
int H = GetSystemMetrics(SM_CYSCREEN);

sf::Font font;
int main()
{
   font.loadFromFile("btn_font.ttf");
    Menu menu(W, H);
    menu.run();
}

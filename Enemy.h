#pragma once
#include "AnimationManager.h"
#include "Level.h"
#include "HealthBar.h"
#include "Bullet.h"
#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

class Enemy {
public:
    int dir, up, hp;
    double x, y, dx, dy, w, h, last_x, last_y, speed;
    AnimationManager anim;
    enum { stay, walk, climb, jump } STATE;
    std::map<std::string, bool> key;

    Enemy(AnimationManager& a, int x_ = 100, int y_ = 100);

    int update(double time, std::vector<Object> a, sf::IntRect p);
    
    void draw(sf::RenderWindow& window);

    bool isCollision(std::vector<Object> a);

    void setBoost();

    void getDamage(int damage);

    bool isDead();

    void shoot(double x_, double y_);

    void setBulletAnim(AnimationManager anim);

    sf::IntRect getRect();

private:
    HealthBar hpBar;
    AnimationManager bullet_anim;
    std::vector<Bullet> bullets;
};


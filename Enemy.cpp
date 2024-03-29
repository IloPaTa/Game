#include "Enemy.h"
#include "Bullet.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <random>
#include<chrono>
#include <SFML/Graphics.hpp>

std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

Enemy::Enemy(AnimationManager& a, int x_, int y_) {
    anim = a;
    x = x_;
    y = y_;
    w = a.getRect().width / a.animList.size() / 4;
    h = a.getRect().height;
    hp = 50;
    last_x = x;
    last_y = y;
    speed = 0.07;
    hpBar.setHp(hp);
    hpBar.setPosition(x, y);
    hpBar.setSize(w, 10);
    STATE = walk;
}

void Enemy::draw(sf::RenderWindow& window) {
    anim.drawAnim(window, x, y);
    hpBar.draw(window);
    for (int i = 0; i < bullets.size(); ++i) {
        bullets[i].anim.drawAnim(window, bullets[i].x, bullets[i].y);
    }
}

int Enemy::update(double time, std::vector<Object> a, sf::IntRect p) {
    extern int W;
    extern int H;
    int get_damage = 0;
    if (STATE == walk)
        anim.set("walk");

    if (dir)
        anim.flip();

    anim.tick(time);
    x += dx * time;
    y += dy * time;

    for (int i = 0; i < bullets.size(); ++i) {
        bullets[i].update(time);
    }

    for (int i = 0; i < bullets.size(); ++i) {
        for (auto j : a) {
            if (bullets[i].isCollision(j.rect)) {
                bullets.erase(bullets.begin() + i);
                --i;
                break;
            }
        }
    }

    for (int i = 0; i < bullets.size(); ++i) {
        if (bullets[i].isCollision(p)) {
            get_damage += bullets[i].damage;
            bullets.erase(bullets.begin() + i);
            --i;
        }
    }
    
    if (isCollision(a)) {
        x = last_x;
        y = last_y;
        setBoost();
    }
    else {
        last_x = x;
        last_y = y;
    }
    hpBar.setPosition(x, y - 100);
    if (rnd() % (1443) == 0) { //37*17*6
        shoot(p.left, p.top);
    }

    return get_damage;
}

sf::IntRect Enemy::getRect() {
    sf::IntRect tmp = anim.getRect();
    if (dir == 1)
        tmp.left = x + 40;
    else
        tmp.left = x;
    tmp.top = y - 75;
    return tmp;
}

bool Enemy::isCollision(std::vector<Object> a) {
    for (auto i : a) {
        if (i.rect.intersects(getRect())) {
            return true;
        }
    }
    return false;
}

void Enemy::setBoost() {
    if (rnd() % 3 == 0 && dx != speed) {
        dir = 1;
        dx = -speed;
        dy = 0;
        return;
    }
    else if (rnd() % 3 == 1 && dx != -speed) {
        dir = 0;
        dx = speed;
        dy = 0;
        return;
    }
    dx = 0;
    if (rnd() % 2 == 1 && dy != -speed) {
        dy = speed;
    }
    else {
        dy = -speed;
    }
}

bool Enemy::isDead() {
    if (hp <= 0) {
        return true;
    }
    return false;
}

void Enemy::getDamage(int damage) {
    hp = hp - damage;
    hpBar.changeHealth(hp);
}

void Enemy::shoot(double x_, double y_) {
    Bullet bull(bullet_anim, x, y, 1);
    bull.setBoost(x_, y_);
    bull.setDamage(5);
    bullets.push_back(bull);
}

void Enemy::setBulletAnim(AnimationManager anim) {
    bullet_anim = anim;
}

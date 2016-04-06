#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include "stdafx.h"
#include "SFML\Graphics.hpp"
#

class Player : public sf::Sprite {
private:
	sf::Texture m_texture;
	float m_speed;
public:
	Player() {
		m_speed = 1.0f;
		m_texture.loadFromFile("Player.png");
		setTexture(m_texture);
		setOrigin(64 / 2, 64 / 2);
		setPosition(250, 250);
	}
	~Player() {		}

	void update() {

	}
	void move(sf::Vector2f dir) {
		sf::Vector2f force = dir * m_speed;
		sf::Vector2f pos = getPosition();
		setPosition(pos + force);
	}
};

#endif
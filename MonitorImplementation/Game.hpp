#ifndef _GAME_HPP
#define _GAME_HPP
#include "stdafx.h"
#include "boost\interprocess\sync\interprocess_mutex.hpp"
#include "SFML\Window.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\System\Clock.hpp"
#include "SFML\System\Time.hpp"
#include "BoundedBuffer.hpp"
#include "Player.hpp"
#include "boost\thread.hpp"
#include <list>

class Game {
private:
	BoundedBuffer *m_buffer;
	sf::Font m_font;
	Player *m_player;
	sf::Clock m_delta_clock;
	sf::RenderWindow* m_window;
	sf::Vector2f m_direction;
	boost::interprocess::interprocess_mutex m_queue_mutex;
	boost::interprocess::interprocess_mutex m_buffer_mutex;
	boost::thread m_queue_thread;
	sf::Text text;
	sf::Vector2f m_start_pos;
	sf::Vector2f m_seperation;
	list<sf::Text> m_display;
	float m_dis_prev;
public:
	bool isRunning;
	Game();
	~Game();
	
	void run();
	void render();
	void handleInput(sf::Event e);
	void generateText(sf::Vector2f dir);

	void queueWorker(BoundedBuffer *m_buffer, list<sf::Text> *m_display, sf::RenderWindow *w);
};

#endif
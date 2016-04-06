#include "stdafx.h"
#include "Game.hpp"

Game::Game() : m_window(new sf::RenderWindow(sf::VideoMode(500, 500, 32), "Monitor Implementation", sf::Style::Titlebar)),
				m_buffer_mutex(), m_queue_mutex()
{
	m_start_pos = sf::Vector2f(10, 10);
	m_seperation = sf::Vector2f(0, 50);

	isRunning = true;
	m_buffer = new BoundedBuffer();
	m_player = new Player();

	text.setColor(sf::Color(0, 0, 0, 255));
	text.setFont(m_font);
	text.setCharacterSize(32);

	//sf::Vector2f pos;
	//pos = m_start_pos + (m_seperation * (float)i);
	//text.setPosition(pos);

	m_font.loadFromFile("Mona Shark.otf");

	m_delta_clock.restart();
	m_queue_thread = boost::thread(boost::bind(&Game::queueWorker, this, m_buffer, &m_display, m_window));
	//boost::bind(&Game::renderHUD, m_buffer, &m_window);
}
Game::~Game() {		
	m_queue_thread.join();

	delete m_player;
	delete m_buffer;
}

void Game::run() {
	while (isRunning) {
		m_direction = sf::Vector2f(0, 0);

		sf::Event l_event;
		m_window->pollEvent(l_event);
		handleInput(l_event);
		m_player->move(m_direction);

		render();
	}
}
void Game::render() {
	m_queue_mutex.lock();
	m_window->clear(sf::Color(100, 149, 237, 255));

	m_window->draw(*m_player);

	for (auto iter = m_display.begin(); iter != m_display.end(); iter++) {
		m_window->draw((*iter));
	}

	m_window->display();
	m_queue_mutex.unlock();
}

void Game::queueWorker(BoundedBuffer *m_buffer, list<sf::Text> *m_display, sf::RenderWindow *w) {
	sf::Vector2f m_start_pos = sf::Vector2f(10, 10);
	sf::Vector2f m_seperation = sf::Vector2f(0, 50);
	while (true) {
		m_queue_mutex.lock();
		//Get anything new off the buffer
		if (m_buffer->count > 0) {
			text.setString(m_buffer->fetch());
			m_display->push_back(text);
		}

		//Pop List if too big
		if (m_display->size() > m_buffer->lenght) {
			m_display->pop_front();
		}

		//Reposition List
		sf::Vector2f textPos = m_start_pos;
		for (auto iter = m_display->begin(); iter != m_display->end(); iter++) {
			(*iter).setPosition(textPos);
			textPos += m_seperation;
		}
		m_queue_mutex.unlock();
	}
}

void Game::handleInput(sf::Event e) {
	sf::Vector2f direction(0, 0);
	if (e.key.code == sf::Keyboard::Escape)
		isRunning = false;

	string key;
	if (e.type == sf::Event::KeyPressed) {
		switch (e.key.code) {
		case sf::Keyboard::Up:
			direction += sf::Vector2f(0, -1);
			break;
		case sf::Keyboard::Down:
			direction += sf::Vector2f(0, 1);
			break;
		case sf::Keyboard::Left:
			direction += sf::Vector2f(-1, 0);
			break;
		case sf::Keyboard::Right:
			direction += sf::Vector2f(1, 0);
			break;
		}
		generateText(direction);
	}
	m_direction += direction;
}

void Game::generateText(sf::Vector2f dir) {
	string s_dir;
	if (dir == sf::Vector2f(0, -1))
		s_dir = "Up";
	if (dir == sf::Vector2f(0, 1))
		s_dir = "Down";
	if (dir == sf::Vector2f(-1, 0))
		s_dir = "Left";
	if (dir == sf::Vector2f(1, 0))
		s_dir = "Right";

	if (m_delta_clock.getElapsedTime().asSeconds() > 0.10f) {
		m_queue_mutex.lock();
		m_buffer->deposit(s_dir);
		m_queue_mutex.unlock();
		m_delta_clock.restart();
	}
}

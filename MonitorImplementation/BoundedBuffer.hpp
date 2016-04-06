#ifndef _BOUNDEDBUFFER_HPP
#define _BOUNDEDBUFFER_HPP
#include "stdafx.h"
#include "boost\interprocess\sync\interprocess_semaphore.hpp"
#include "SFML\Graphics\Text.hpp"
#include "SFML\Graphics.hpp"
#include <memory>

//Simple Bounded buffer implementation
class BoundedBuffer {
public:
	int lenght = 6;			//Lenght of the buffer
	string buffer[6];		//The actual array to store everything put into the buffer

	int front = 0;
	int rear = 0;
	int count = 0;

	boost::interprocess::interprocess_semaphore not_full;
	boost::interprocess::interprocess_semaphore not_empty;

	BoundedBuffer() : not_full(0), not_empty(0) {
		
	}
	void deposit(string rec) {
		while (count == lenght - 1) {
			not_full.wait();
		}
		
		buffer[rear] = rec;
		if (rear < lenght)
			rear++;
		if (rear == lenght)
			rear = 0;
		count++;
		not_empty.post();
	}	
	string fetch() {
		while (count == 0) {
			not_empty.wait();
		}
		string result = buffer[front];

		if (front < lenght)
			front = (front + 1) % lenght;
		if (front == lenght)
			front = 0;
		count--;
		not_full.post();
		//buffer[front] = "";
		return result;
	}

	/*void render(sf::RenderWindow &w) {
		for (int i = 0; i < lenght; i++) {
			w.draw(buffer[i]);
		}
	}*/
};

#endif
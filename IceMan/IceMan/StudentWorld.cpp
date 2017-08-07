#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{

	for (int x = 0; x < oilFieldX; x++) { //create the oil field covered by ice
		
	
		if (x < 30 || x > 33) {
			for (int y = 0; y < oilFieldY; y++) {
				Ice *newIce = new Ice(x, y);
				m_oilField[x][y] = newIce;
			}
		}

		else  {
			for (int y = 0; y < oilFieldY; y++) {
				if (y >= 0 && y < 4) {
					Ice *newIce = new Ice(x, y);
					m_oilField[x][y] = newIce;
				}
				else
					m_oilField[x][y] = nullptr;
			}
		}

		
	}
	m_iceman = new Iceman();
	m_iceman->setWorld(this);



	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();
	//return GWSTATUS_PLAYER_DIED;
	m_iceman->doSomething();

	if (m_iceman->isRemoveIce())
		removeIce(m_iceman->getX(), m_iceman->getY());

	return GWSTATUS_CONTINUE_GAME;
}



bool StudentWorld::isCoveredByIce(const int x,const int y) {
	if (x >= 63 || y >= 59)
		return false;
	for (int i = x; i < std::min(x + 3, 63);i++) {
		for (int j = y; j < std::min(y + 3, 59); j++) {
			if (m_oilField[i][j] != nullptr)
				return true;
		}
	}
	return false;
}



void StudentWorld::removeIce(const int x,const int y) {
	for (int i = x; i < std::min(x+3, 63); i++) {
		for (int j = y; j < std::min(y+3, 59); j++) {
			if (m_oilField[i][j] != nullptr) {
				//Ice* temp = m_oilField[i][j];
				m_oilField[i][j]->setVisible(false);
				//delete temp;
			}
		}
	}
	
	
}
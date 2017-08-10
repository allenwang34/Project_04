#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include <random>
#include <math.h>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


StudentWorld::StudentWorld(std::string assetDir)
	:GameWorld(assetDir) { }


StudentWorld::~StudentWorld() {
	delete m_iceman;
	for (int x = 0; x < oilFieldX; x++) {
		for (int y = 0; y < oilFieldY; y++) {
			if (m_oilField[x][y] != nullptr)
				delete m_oilField[x][y];
		}
	}
	list<Actor*>::iterator it = m_gameObjectList.begin();
	while (it != m_gameObjectList.end()) {
		delete *it;
		it = m_gameObjectList.erase(it);
	}
}

int StudentWorld::init()
{
	
	m_currentLevel = getLevel();
	m_oilNum = std::min(2 + m_currentLevel, 21);
	m_boulderNum = std::min(m_currentLevel / 2 + 2, 9);
	m_goldNum = std::max(5 - m_currentLevel / 2, 2);


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
				else {
					Ice *newIce = new Ice(x, y);
					m_oilField[x][y] = newIce;
					newIce->setVisible(false);
				}
			}
		}
	}



	m_iceman = new Iceman(); // create Iceman
	m_iceman->setWorld(this);

	for (int i = 0; i < m_boulderNum; i++) {
		
		if (m_gameObjectList.empty()) { //if the object list is empty, just create game object at random position
			int randX = getRandNum(0, 60);
			int randY = getRandNum(20, 56);
			removeIce(randX, randY);
			Boulder* newBoulder = new Boulder(randX, randY);
			newBoulder->setWorld(this);
			m_gameObjectList.push_back(newBoulder);
		}
		else { //if the game object is not empty
			list<Actor*>::iterator it = m_gameObjectList.begin();
			int randX = getRandNum(0, 60); //get random x and random y value
			int randY = getRandNum(20, 56);
			while (it != m_gameObjectList.end()) { // compare the distance with every element in the list
				int objectX = (*it)->getX();
				int objectY = (*it)->getY();
				double distance = sqrt(pow((objectX - randX), 2) + pow((objectY - randY), 2));
				if (distance < 6) { //if the distance is within 6 
					regenRandNum(randX, randY, 0, 60, 20, 56); //re-generate x and y 
					it = m_gameObjectList.begin(); //compare the entire list again 
				}
				it++; //keep comparing the next element in the list
			}
			//after thi while loop, the x and y value are valid to use
			Boulder* newBoulder = new Boulder(randX, randY); //create game object at x and y
			newBoulder->setWorld(this);
			m_gameObjectList.push_back(newBoulder);
			removeIce(randX, randY);
		}
	}




	setGameStatText("Level: " + std::to_string(getLevel()) + " Lives: " + std::to_string(getLives()) + " Hlth: " 
		+ std::to_string(m_iceman->GetHealth()) +"%" + " Wtr: " + std::to_string(m_iceman->GetWaterAmount())
		+ " Gld: " + std::to_string(m_iceman->GetGold()) + " Oil Left: " + std::to_string(m_oilNum) + " Sonar: " 
		+ std::to_string(m_iceman->GetSonar()) + " Score: " + std::to_string(getScore()));
	return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();
	//return GWSTATUS_PLAYER_DIED;

	setGameStatText("Level: " + std::to_string(getLevel()) + " Lives: " + std::to_string(getLives()) + " Hlth: "
		+ std::to_string(m_iceman->GetHealth()) + "%" + " Wtr: " + std::to_string(m_iceman->GetWaterAmount())
		+ " Gld: " + std::to_string(m_iceman->GetGold()) + " Oil Left: " + std::to_string(m_oilNum) + " Sonar: "
		+ std::to_string(m_iceman->GetSonar()) + " Score: " + std::to_string(getScore()));

	m_iceman->doSomething();

	if (m_iceman->isRemoveIce()) {
		removeIce(m_iceman->getX(), m_iceman->getY());
	}
	if (getScore() > 1000) {
		increaseScore(-1000);
		return GWSTATUS_FINISHED_LEVEL;
	}

	list<Actor*>::iterator it = m_gameObjectList.begin();
	for (int i = 0; i < m_boulderNum; i++) {
		if ((*it != nullptr))
			(*it)->doSomething();
		it++;
	}

	cleanDeadObjects();
	return GWSTATUS_CONTINUE_GAME;
}



bool StudentWorld::isCoveredByIce(const int x,const int y) {
	if (y >= 59)
		return false;
	for (int i = x; i < std::min(x + 4, 64);i++) {
		for (int j = y; j < std::min(y + 4, 60); j++) {
			if (m_oilField[i][j] != nullptr && m_oilField[i][j]->isVisible() != false)
				return true;
		}
	}
	return false;
}

bool StudentWorld::isBottomCoveredByIce(const int x, const int y) {
	if (m_oilField[x][y - 1]->isVisible()==false && m_oilField[x + 1][y - 1]->isVisible() == false
		&& m_oilField[x + 2][y - 1]->isVisible()==false && m_oilField[x + 3][y - 1]->isVisible()==false)
		return false;
	else
		return true;
}

bool StudentWorld::isBottomAnotherBoulder(const int x, const int y) {

	list<Actor*>::iterator it = m_gameObjectList.begin();
	for (int i = 0; i < m_boulderNum; i++) {
		if ( (*it) != nullptr && y - 1 == (*it)->getY())
			return true;
		it++;
	}
	return false;
}


void StudentWorld::removeIce(const int x,const int y) {
	for (int i = x; i < std::min(x+4, 64); i++) {
		for (int j = y; j < std::min(y+4, 60); j++) {
			if (m_oilField[i][j] != nullptr) {
				m_oilField[i][j]->setVisible(false);
				increaseScore(1);
			}
		}
	}
	
	
}



void StudentWorld::cleanUp() {
	delete m_iceman;
	for (int x = 0; x < oilFieldX; x++) {
		for (int y = 0; y < oilFieldY; y++) {
			if (m_oilField[x][y] != nullptr)
				delete m_oilField[x][y];
		}
	}
	list<Actor*>::iterator it = m_gameObjectList.begin();
	while (it != m_gameObjectList.end()) {
			delete *it;
			it = m_gameObjectList.erase(it);
	}
}


int StudentWorld::getRandNum(int min, int max) { //from https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased

	int random_integer = uni(rng);
	return random_integer;
}


void StudentWorld::regenRandNum(int &X, int&Y, int Xmin, int Xmax, int Ymin, int Ymax) {
	X = getRandNum(Xmin, Xmax);
	Y = getRandNum(Ymin, Ymax);
}

bool StudentWorld::isBoulderAhead(const int x, const int y) {

	list<Actor*>::iterator it = m_gameObjectList.begin();
	for (int i = 0; i < m_boulderNum; i++) {
		if (*it != nullptr) {
			int boulderX = (*it)->getX();
			int boulderY = (*it)->getY();
			for (int i = x; i < std::min(x + 4, 64); i++) {
				for (int j = y; j < std::min(y + 4, 60); j++) {
					if (x >= boulderX - 2 && x <= boulderX + 2 && y >= boulderY - 2 && y <= boulderY + 2)
						return true;
				}
			}
		}
		it++;
	}
	return false;

}

void StudentWorld::cleanDeadObjects() {
	list<Actor*>::iterator it = m_gameObjectList.begin();
	//for (int i = 0; i < m_boulderNum; i++) {
	while (it != m_gameObjectList.end()) {
		if ((*it) != nullptr && (*it)->getAlive() == false) {
			Actor* temp = *it;
			*it = nullptr;
			delete temp;
		}
		it++;
	}
}
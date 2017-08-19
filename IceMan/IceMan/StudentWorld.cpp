#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <algorithm>
#include <random>
#include <math.h>
#include <queue>
#include <tuple>
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
	m_GoodiePossibility = (m_currentLevel * 25 + 300);
	m_isSonarKitInField = false;
	maxProtestorNum =std::min(15, 2 + m_currentLevel * 2);
	m_protestorTick =std::max(25, 200 - m_currentLevel);
	m_protestorNum = 0;
	m_protestorCounter = 0;
	m_hardCorePossiblity = std::min(90, m_currentLevel * 10 + 30);
	

	for (int x = 0; x < oilFieldX; x++) { //create the oil field covered by ice
		if (x < 30 || x > 33) {
			for (int y = 0; y < oilFieldY; y++) {
				if (y < 60) {
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

	for (int i = 0; i < oilFieldX; i++) {  //initialize the steps array
		/*for (int j = 0; j < oilFieldY; j++) {
			m_stepArr[i][j] = 99999;
		}*/
		for (int x = 0; x < oilFieldX; x++) {
			if (x < 30 || x > 33) {
				for (int y = 0; y < oilFieldY; y++) {
					if (y < 60) {
						m_stepArr[x][y] = 99999;//wall
					}
					else {
						m_stepArr[x][y] = 99998;//walkway
					}
				}

			}
			else {
				for (int y = 0; y < oilFieldY; y++) {

					if (y >= 0 && y < 4) {
						m_stepArr[x][y] = 99999;
					}
					else {
						m_stepArr[x][y] = 99998;
					}
				}
			}
		}
	}



	m_iceman = new Iceman(); // create Iceman
	m_iceman->setWorld(this);


	for (int i = 0; i < m_boulderNum; i++) { //create boulders
		setRandXY(0, 60, 20, 56);
		removeIce(m_randXY[0], m_randXY[1]);
		Boulder* newBoulder = new Boulder(m_randXY[0], m_randXY[1]);
		newBoulder->setWorld(this);
		m_gameObjectList.push_back(newBoulder);
	}

	for (int i = 0; i < m_oilNum; i++) {
		setRandXY(0, 63, 0, 59);
		Oil* newOil = new Oil(m_randXY[0], m_randXY[1]);
		newOil->setWorld(this);
		m_gameObjectList.push_back(newOil);
	}


	for (int i = 0; i < m_goldNum; i++) {
		setRandXY(0, 63, 0, 59);
		Gold* newGold = new Gold(m_randXY[0], m_randXY[1], true);
		newGold->setWorld(this);
		m_gameObjectList.push_back(newGold);
	}

	RegularProtestor* m_regularProt = new RegularProtestor;
	m_regularProt->setWorld(this);
	m_regularProt->setTicksToWait();
	m_gameObjectList.push_back(m_regularProt);
	m_protestorNum++;


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

	if (!m_iceman->getAlive()) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}


	if (m_oilNum == 0) {
		return GWSTATUS_FINISHED_LEVEL;
	}


	setGameStatText("Level: " + std::to_string(getLevel()) + " Lives: " + std::to_string(getLives()) + " Hlth: "
		+ std::to_string(m_iceman->GetHealth()) + "%" + " Wtr: " + std::to_string(m_iceman->GetWaterAmount())
		+ " Gld: " + std::to_string(m_iceman->GetGold()) + " Oil Left: " + std::to_string(m_oilNum) + " Sonar: "
		+ std::to_string(m_iceman->GetSonar()) + " Score: " + std::to_string(getScore()));


	if (m_protestorNum < maxProtestorNum && m_protestorCounter && m_protestorCounter >=  m_protestorTick && m_protestorNum < maxProtestorNum) {
		if (getRandNum(1, m_hardCorePossiblity) == m_hardCorePossiblity) {
			HardCoreProtestor* m_hardProt = new HardCoreProtestor;
			m_hardProt->setWorld(this);
			m_hardProt->setTicksToWait();
			m_gameObjectList.push_back(m_hardProt);
			m_protestorNum++;
			resetProtestorCounter();
		}
		else {
			RegularProtestor* m_regularProt = new RegularProtestor;
			m_regularProt->setWorld(this);
			m_regularProt->setTicksToWait();
			m_gameObjectList.push_back(m_regularProt);
			m_protestorNum++;
			resetProtestorCounter();
		}
	}
	else {
		m_protestorCounter++;
	}

	if (getRandNum(1, m_GoodiePossibility) == m_GoodiePossibility) {
		if (getRandNum(1, 5) != 5) {
			setRandXY(0,63,0,59);
			int objectX = m_randXY[0];
			int objectY = m_randXY[1];
			WaterPool *newWaterPool = new WaterPool(objectX,objectY);
			newWaterPool->setWorld(this);
			m_gameObjectList.push_back(newWaterPool);
		}
		else {
			SonarKit* newSonarKit = new SonarKit(0, 60);
			newSonarKit->setWorld(this);
			m_gameObjectList.push_back(newSonarKit);
		}
	}


	m_iceman->doSomething();
	
	list<Actor*>::iterator it = m_gameObjectList.begin(); //ask every object in the list, if not dead, do something. 
	while (it!=m_gameObjectList.end()) {
		if ((*it != nullptr))
			(*it)->doSomething();
		it++;
	}

	

	cleanDeadObjects(); //clean all the dead objects 
	return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::setProtestorLeave() {
	list<Actor*>::iterator it = m_gameObjectList.begin(); //ask every object in the list, if not dead, do something. 
	while (it != m_gameObjectList.end()) {
		if ((*it != nullptr))
			(*it)->getAnnoyed();
		it++;
	}
}

void StudentWorld::revealGoodiesAround(int playerX, int playerY) {
	list<Actor*>::iterator it = m_gameObjectList.begin();
	while (it != m_gameObjectList.end()) {
		if (*it != nullptr) {
			int objectX = (*it)->getX();
			int objectY = (*it)->getY();
			if (sqrt(pow((playerX - objectX), 2) + pow((playerY - objectY), 2)) <= 12) {
				(*it)->setVisible(true);
			}
		}
		it++;
	}
}


void StudentWorld::setRandXY(int xMin, int xMax, int yMin, int yMax) {
	if (m_gameObjectList.empty()) { //if the object list is empty, just create game object at random position
		int randX = getRandNum(xMin, xMax);
		int randY = getRandNum(yMin, yMax);
		m_randXY[0] = randX;
		m_randXY[1] = randY;
		
	}
	else { //if the game object is not empty
		list<Actor*>::iterator it = m_gameObjectList.begin();
		int randX = getRandNum(0, 59); //get random x and random y value
		int randY = getRandNum(20, 56);
		while (it != m_gameObjectList.end()) { // compare the distance with every element in the list
			if (*it != nullptr) {
				int objectX = (*it)->getX();
				int objectY = (*it)->getY();
				double distance = sqrt(pow((objectX - randX), 2) + pow((objectY - randY), 2));
				if (distance < 6) { //if the distance is within 6 
					regenRandNum(randX, randY, 0, 60, 20, 56); //re-generate x and y 
					it = m_gameObjectList.begin(); //compare the entire list again 
				}
				else {
					it++; //keep comparing the next element in the list
				}
			}
			else {
				it++;
			}
		}
		//after thi while loop, the x and y value are valid to use
		m_randXY[0] = randX;
		m_randXY[1] = randY;
	}



}



void StudentWorld::dropGold(int startX, int startY) {
	Gold* newGold = new Gold(startX, startY, false);
	newGold->setWorld(this);
	m_gameObjectList.push_back(newGold);
}

void StudentWorld::shootWaterSquirt(int bornX, int bornY) {
	int SquirtBornX = bornX;
	int SquirtBornY = bornY;
	if (!isCoveredByIce(SquirtBornX, SquirtBornY) && !isBoulderAhead(SquirtBornX, SquirtBornY) && bornX >= 3 && bornX <= 60 && bornY >= 3 && bornY <= 60) {
		Squirt *newSquirt = new Squirt(SquirtBornX, SquirtBornY, m_iceman->getDirection());
		newSquirt->setWorld(this);
		m_gameObjectList.push_back(newSquirt);
	}
}

bool StudentWorld::isIcemanNearBy(const int x, const int y, const int dist) {
	int icemanX = m_iceman->getX();
	int icemanY = m_iceman->getY();
	if (sqrt(pow((icemanX - x), 2) + pow((icemanY - y), 2)) <= dist)
		return true;
	else
		return false;
}


bool StudentWorld::isCoveredByIce(const int x,const int y) {
	if (y >= 60)
		return false;
	for (int i = x; i < std::min(x + 4, 64);i++) {
		for (int j = y; j < std::min(y + 4, 60); j++) {
			if (m_oilField[i][j] != nullptr && m_oilField[i][j]->isVisible() != false)
				return true;
		}
	}
	return false;
}

bool StudentWorld::isIceInLineX(int IcemanX, int ProtesterX, int Y) {
	if (Y >= 60)
		return false;
	for (int x = ProtesterX; x < IcemanX; x++) {
		for (int i = x; i < std::min(x + 4, 64); i++) {
			for (int j = Y; j < std::min(Y + 4, 60); j++) {
				if (m_oilField[i][j] != nullptr && m_oilField[i][j]->isVisible() != false)
					return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isIceInLineY(int IcemanY, int ProtesterY, int X) {
	for (int y = ProtesterY; y < IcemanY; y++) {
		for (int i = X; i < std::min(X + 4, 64); i++) {
			for (int j = y; j < std::min(y + 4, 60); j++) {
				if (m_oilField[i][j] != nullptr && m_oilField[i][j]->isVisible() != false)
					return true;
			}
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

bool StudentWorld::isBottomIceman(const int x, const int y) {
	if (y - 1 == m_iceman->getY()) {
		int icemanX = m_iceman->getX();
		if (icemanX - x >= 0 && icemanX - x <= 3)
			return true;
	}
	return false;
}

bool StudentWorld::isBottomAnotherBoulder(const int x, const int y) {

	list<Actor*>::iterator it = m_gameObjectList.begin();
	for (int i = 0; i < m_boulderNum; i++) {
		if ((*it) != nullptr && y - 1 == (*it)->getY()) {
			int objectX = (*it)->getX();
			if(objectX-x>=0 && objectX-x<=3)
				return true;
		}
		it++;
	}
	return false;
}

void StudentWorld::removeIce(const int x, const int y) {
	for (int i = x; i < std::min(x+4, 64); i++) {
		for (int j = y; j < std::min(y+4, 64); j++) {
			if (m_oilField[i][j] != nullptr) {
				m_oilField[i][j]->setVisible(false);
				m_stepArr[i][j] = 99998;
				generateStepArr();
			}
		}
	}
}

bool StudentWorld::isLocationDiscovered(std::tuple<int, int> xyCoords, const std::vector< std::tuple<int, int>> locations) {
	if (locations.empty())
		return false;
	for (unsigned int i = 0; i < locations.size(); i++) {
		if (xyCoords == locations[i])
			return true;
	}
	return false;
}




void StudentWorld::generateStepArr() {
	for (int i = 0; i < oilFieldX; i++) {
		for (int j = 0; j < oilFieldY; j++) {
			if (m_oilField[i][j]->isVisible()==false) { //!m_oilField[i][j]->isVisible()

				m_stepArr[i][j] = 60 - i + 60 - j;
				//Below is my Maze Searching algorithm, but it seems don't work well.
				/*int stepCounter = 0;
				std::queue<tuple<int, int>> locationQueue;
				std::vector<tuple<int, int>> visitedLocations;
				locationQueue.push(std::tuple<int, int>(i, j));
				//bool notBreak = true;
				while (!locationQueue.empty()) { //!locationQueue.empty()
					//if (locationQueue.empty())
						//return;
					if (locationQueue.front() == std::tuple<int, int>(60, 60)) {
						m_stepArr[i][j] = stepCounter;
						break;
						//notBreak = false;
					}
					else {
						int x = std::get<0>(locationQueue.front());
						int y = std::get<1>(locationQueue.front());
						locationQueue.pop();
						if (x - 1 >= 0 && m_stepArr[x - 1][y] != 99999 && !isLocationDiscovered(std::tuple<int, int>(x - 1, y), visitedLocations)) {
							visitedLocations.push_back(std::tuple<int, int>(x - 1, y));
							locationQueue.push(std::tuple<int, int>(x - 1, y));
						}
						if (x + 1 <= 60 && m_stepArr[x + 1][y] != 99999 && !isLocationDiscovered(std::tuple<int, int>(x + 1, y), visitedLocations)) {
							visitedLocations.push_back(std::tuple<int, int>(x + 1, y));
							locationQueue.push(std::tuple<int, int>(x + 1, y));
						}
						if (y-1 >= 0 && m_stepArr[x][y-1] != 99999 && !isLocationDiscovered(std::tuple<int, int>(x, y-1), visitedLocations)) {
							visitedLocations.push_back(std::tuple<int, int>(x, y-1));
							locationQueue.push(std::tuple<int, int>(x, y-1));
						}
						if (y + 1 <= 60 && m_stepArr[x][y + 1] != 99999 && !isLocationDiscovered(std::tuple<int, int>(x, y + 1), visitedLocations)) {
							visitedLocations.push_back(std::tuple<int, int>(x, y + 1));
							locationQueue.push(std::tuple<int, int>(x, y + 1));
						}
						stepCounter++;
					}
				}*/
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

bool StudentWorld::isObjectAhead(const int x, const int y) {
	list<Actor*>::iterator it = m_gameObjectList.begin();
	while (it != m_gameObjectList.end()) {
		if (*it != nullptr) {
			int objX = (*it)->getX();
			int objY = (*it)->getY();
			if (sqrt(pow((x - objX), 2) + pow((y - objY), 2)) <= 3) {
				(*it)->getAnnoyed();
				return true;
			}
		}
		it++;
	}
	return false;

}

void StudentWorld::cleanDeadObjects() {
	list<Actor*>::iterator it = m_gameObjectList.begin();
	while (it != m_gameObjectList.end()) {
		if ((*it) != nullptr && (*it)->getAlive() == false) {
			Actor* temp = *it;
			*it = nullptr;
			delete temp;
		}
		it++;
	}

}

int StudentWorld::getSmallestValue(const int a,const int b,const int c,const int d) {
	int minOfFirstSet = std::min(a, b);
	int minOfSecondSet = std::min(c, d);
	int smallestValue = std::min(minOfFirstSet, minOfSecondSet);
	return smallestValue;
}

std::tuple<int, int> StudentWorld::getNextStep(const int xCoord,const int yCoord) {
	int a, b, c, d;
	if (xCoord - 1 >= 0 ) {
		a = m_stepArr[xCoord - 1][yCoord];
	}
	else {
		a = 99999;
	}
	if (xCoord + 1 <= 60 && m_stepArr[xCoord+4][yCoord] !=99999 ) {
		b = m_stepArr[xCoord + 1][yCoord];
	}
	else if (m_stepArr[xCoord + 4][yCoord] == 99999) {
		for (int i = 1; i <= 4; i++) {
			m_stepArr[xCoord + i][yCoord] = 99999;
			b = 99999;
		}
	}
	else {
		b = 99999;
	}
	if (yCoord - 1 >= 0) {
		c = m_stepArr[xCoord][yCoord - 1];
	}
	else {
		c = 99999;
	}
	if (yCoord + 1 <= 60) {
		d = m_stepArr[xCoord][yCoord + 1];
	}
	else {
		d = 99999;
	}
	int smallestValue = getSmallestValue(a, b, c, d);
	
	
	if (m_stepArr[xCoord - 1][yCoord] == smallestValue) {
		return std::tuple<int, int>(xCoord - 1, yCoord);
	}
	else if (m_stepArr[xCoord + 1][yCoord] == smallestValue) {
		return std::tuple<int, int>(xCoord + 1, yCoord);
	}
	else if (m_stepArr[xCoord][yCoord - 1] == smallestValue) {
		return std::tuple<int, int>(xCoord, yCoord - 1);
	}
	else if (m_stepArr[xCoord][yCoord + 1] == smallestValue)
		return std::tuple<int, int>(xCoord, yCoord + 1);
	else
		return std::tuple<int, int>(xCoord, yCoord); 

}
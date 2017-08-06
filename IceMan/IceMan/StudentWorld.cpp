#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

bool StudentWorld::isCoveredByIce(const int x,const int y) {
	if (x >= 63 || y >= 59)
		return false;
	else if (m_oilField[x][y] != nullptr)
		return true;
	else
		return false;
}

void StudentWorld::removeIce(const int x,const int y) {
	Ice* temp = m_oilField[x][y];
	m_oilField[x][y] = nullptr;
	delete temp;
}
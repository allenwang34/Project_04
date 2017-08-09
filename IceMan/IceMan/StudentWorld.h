#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>



// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	 ~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool isCoveredByIce(const int x,const int y);
	void removeIce(const int x,const int y);
	

private:

	const int oilFieldX = 63;
	const int oilFieldY = 59;
	Ice* m_oilField[63][59];
	Iceman* m_iceman;
	int m_currentLevel;
	int m_oilNum;
	int m_boulderNum;
	int m_goldNum;
	std::list<Actor*> m_gameObjectList;
	int getRandNum(int min, int max);
	void regenRandNum(int &X, int&y, int Xmin, int Xmax, int Ymin, int Ymax);
	
};

#endif // STUDENTWORLD_H_

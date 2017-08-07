#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GameController.h"
#include "Actor.h"
#include <string>
#include <list>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual ~StudentWorld();
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
	std::list<Actor*> gameObjects;
	int m_levelCount;
	int m_lives;
	int m_oilLeft;
	int m_score;
	
	



};

#endif // STUDENTWORLD_H_

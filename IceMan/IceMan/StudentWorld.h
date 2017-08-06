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
	StudentWorld(std::string assetDir)
		:GameWorld(assetDir) { 
	}
		

	virtual int init()
	{	
		
		for (int x = 0; x < oilFieldX; x++) { //create the oil field covered by ice
			if (x < 30 || x > 33) {
				for (int y = 0; y < oilFieldY; y++) {
					Ice *newIce = new Ice(x, y);
					m_oilField[x][y] = newIce;
				}
			}
			else {
				for (int y = 0; y < 4; y++) {
					Ice *newIce = new Ice(x, y);
					m_oilField[x][y] = newIce;
				}
			}
		}
		m_iceman = new Iceman();
		

		
		return GWSTATUS_CONTINUE_GAME;
	}

	virtual int move()
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

	virtual void cleanUp()
	{
	}

	
	bool isCoveredByIce(const int x,const int y);

	void removeIce(const int x,const int y);

private:

	const int oilFieldX = 63;
	const int oilFieldY = 59;
	Ice* m_oilField[63][59];
	Iceman* m_iceman;
	std::list<Actor*> gameObjects;
	
	



};

#endif // STUDENTWORLD_H_

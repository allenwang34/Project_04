#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
#include <vector>
#include <tuple>


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
	bool isBoulderAhead(const int x, const int y);
	bool isBottomCoveredByIce(const int x, const int y);
	bool isBottomAnotherBoulder(const int x, const int y);
	bool isBottomIceman(const int x, const int y);
	bool isIcemanNearBy(const int x, const int y, const int dist);
	void decOilNum() { m_oilNum--; }
	Iceman* getPlayer() { return m_iceman; }
	void shootWaterSquirt(int bornX, int bornY);
	void dropGold(int startX, int startY);
	void setRandXY(int xMin, int xMax, int yMin, int yMax);
	int getCurrentLevel() const { return m_currentLevel; }
	void revealGoodiesAround(int playerX, int playerY);
	void sonarPickedUp() { m_isSonarKitInField = false; }

private:
	const int oilFieldX = 64;
	const int oilFieldY = 64;
	Ice* m_oilField[64][64];
	Iceman* m_iceman;
	int m_currentLevel;
	int m_oilNum;
	int m_boulderNum;
	int m_goldNum;
	int m_GoodiePossibility;
	std::list<Actor*> m_gameObjectList;
	int getRandNum(int min, int max);
	void regenRandNum(int &X, int&y, int Xmin, int Xmax, int Ymin, int Ymax);
	void cleanDeadObjects();
	int m_randXY[2];
	bool m_isSonarKitInField;
	int m_stepArr[64][64];
	void generateStepArr();
	bool isLocationDiscovered(const std::tuple<int,int> xyCoords, const std::vector< std::tuple<int, int>> locations);
	std::tuple<int, int> getNextStep(const int xCoord,const int yCoord);
	int getSmallestValue(const int a,const int b,const int c,const int d);
	
	
};

#endif // STUDENTWORLD_H_

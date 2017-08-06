#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth) 
	: GraphObject(imageID,startX,startY,startDirection,size,depth) {
	setVisible(true);

}


Iceman::Iceman()
	: Actor(IID_PLAYER,30,60,right,1.0,0) {
	
	m_hitPoints = 10;
	m_waterAmmo = 5;
	m_sonarCharge = 1;
	m_goldNuggest = 0;
	
}
#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth) 
	: GraphObject(imageID,startX,startY,startDirection,size,depth) {
	setVisible(true);
}

Actor::~Actor() {}

Ice::Ice(int Startx, int Starty)
	: Actor(IID_ICE, Startx, Starty, right, 0.25, 3) {
}

void Ice::doSomething() {
	return;
}

Ice::~Ice() {}



Iceman::Iceman()
	: Actor(IID_PLAYER,30,60,right,1.0,0) {
	
	m_hitPoints = 10;
	m_waterAmmo = 5;
	m_sonarCharge = 1;
	m_goldNuggest = 0;
	m_needRemoveIce = false;
	
}


void Iceman::doSomething() {


	int i;
	if (getWorld()->getKey(i)) {
		switch (i)
		{
		case KEY_PRESS_UP:
			if (getDirection() == up) {
				if (getY() + 1 > 59)
					moveTo(getX(), getY());
				else
					moveTo(getX(), getY() + 1);
			}
			else
				setDirection(up);
			break;

		case KEY_PRESS_DOWN:
			if (getDirection() == down) {
				if (getY() - 1 < 0)
					moveTo(getX(), getY());
				else
					moveTo(getX(), getY() - 1);
			}
			else
				setDirection(down);
			break;
			
		case KEY_PRESS_LEFT:
			if (getDirection() == left) {
				if (getX() - 1 < 0)
					moveTo(getX(), getY());
				else
					moveTo(getX() - 1, getY());
			}
			else
				setDirection(left);
			break;

		case KEY_PRESS_RIGHT:
			if (getDirection() == right) {
				if (getX()+1>60)
					moveTo(getX(), getY());
				else
					moveTo(getX() + 1, getY());
			}
			else
				setDirection(right);
			break;

		default:
			break;
		}
	}


	if (getWorld()->isCoveredByIce(getX(), getY()))
		m_needRemoveIce = true;
	else
		m_needRemoveIce = false;
	
}

Iceman::~Iceman() {
	int m_hitPoints = 0;
	int m_waterAmmo = 0;
	int m_sonarCharge = 0;
	int m_goldNuggest = 0;
}


#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth) 
	: GraphObject(imageID,startX,startY,startDirection,size,depth) {
	setVisible(true);
	setAlive(true);
	setIsAnnoyed(false);
}

Actor::~Actor() {}

Ice::Ice(int Startx, int Starty)
	: Actor(IID_ICE, Startx, Starty, right, 0.25, 3) {
}


Ice::~Ice() {}

Iceman::Iceman()
	: Actor(IID_PLAYER,30,60,right,1.0,0) {
	
	m_hitPoints = 10;
	m_waterAmmo = 5;
	m_sonarCharge = 1;
	m_goldNuggest = 0;
	m_needRemoveIce = false;
	m_isShoot = false;
	
}

void Iceman::doSomething() {

	if (!getAlive())
		return;

	m_isShoot = false;

	int i;
	if (getWorld()->getKey(i)) {
		switch (i)
		{
		case KEY_PRESS_UP:
			if (getDirection() == up) {
				if (getY() + 1 > 60)
					moveTo(getX(), getY());
				else if (getWorld()->isBoulderAhead(getX(),getY()+1))
					break;
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
				else if (getWorld()->isBoulderAhead(getX(), getY()-1))
					break;
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
				else if (getWorld()->isBoulderAhead(getX()-1, getY()))
					break;
				else
					moveTo(getX() - 1, getY());
			}
			else
				setDirection(left);
			break;

		case KEY_PRESS_RIGHT:
			if (getDirection() == right) {
				if (getX()+1>61)
					moveTo(getX(), getY());
				else if (getWorld()->isBoulderAhead(getX()+1, getY()))
					break;
				else
					moveTo(getX() + 1, getY());
			}
			else
				setDirection(right);
			break;

		case KEY_PRESS_SPACE:
			m_isShoot = true;

		default:
			break;
		}
	}

	if (getWorld()->isCoveredByIce(getX(), getY())) {
		m_needRemoveIce = true;
		GameController::getInstance().playSound(SOUND_DIG);
	}
	else
		m_needRemoveIce = false;


	
}

void Iceman::getAnnoyed() { return; }

Iceman::~Iceman() { }

Boulder::Boulder(int startX, int startY) 
	: Actor(IID_BOULDER, startX, startY, down, 1, 1) { 
	m_tickCounter = 0;
	m_waitStart = 0;
	m_state = stable;
}

Boulder::~Boulder() {}


void Boulder::checkState(State &state) {
	int BoulderX = getX();
	int BoulderY = getY();
	if (getWorld()->isBottomCoveredByIce(BoulderX, BoulderY)) {
		state = stable;
	}
	else if (!getWorld()->isBottomCoveredByIce(BoulderX, BoulderY) && isWaiting()) {
		state = waiting;
	}
	else
		state = falling;
}

bool Boulder::isWaiting() {
	if (m_waitStart == 0)
		m_waitStart = m_tickCounter;
	else {
		if (m_tickCounter > m_waitStart + 30)
			return false;
	}
	return true;
}

bool Boulder::isStopFalling() {
	int BoulderX = getX();
	int BoulderY = getY();
	if (getWorld()->isBottomCoveredByIce(BoulderX, BoulderY))
		return true;
	else if (BoulderY == 0)
		return true;
	else if (getWorld()->isBottomAnotherBoulder(BoulderX, BoulderY))
		return true;
	else
		return false;
}

void Boulder::doSomething() {
	if (!getAlive())
		return;
	
	if (m_state != falling)
		checkState(m_state);
	switch (m_state) {
	case stable:
		break;
	case waiting:
		break;
	case falling:
		if (isStopFalling()) {
			setVisible(false);
			setAlive(false);
			break;
		}
		GameController::getInstance().playSound(SOUND_FALLING_ROCK);
		moveTo(getX(), getY() - 1);
		break;
	default:
		break;
	}

	m_tickCounter++;
}

Squirt::Squirt(int startX, int startY, Direction startDir) :Actor(IID_WATER_SPURT, startX, startY, startDir , 1, 1) {

}

Squirt::~Squirt() {}
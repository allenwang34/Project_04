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

ActivatingObject::ActivatingObject(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth) 
	: Actor(imageID, startX,startY,startDirection,size,depth) {
	setActive(false);
	setVisible(false);
}

bool ActivatingObject::isRevealItself() {
	if (!isVisible() && getWorld()->isIcemanNearBy(getX(), getY(), 4)) {
		setVisible(true);
		return true;
	}
	return false;
}

bool ActivatingObject::isPickup() {
	if (isVisible() && getWorld()->isIcemanNearBy(getX(), getY(), 4)) {
		setActive(true);
		setAlive(false);
		return true;
	}
	return false;
}

ActivatingObject::~ActivatingObject() {}

Gold::Gold(int startX, int startY, bool isPermanent) 
	: ActivatingObject(IID_GOLD, startX, startY, right, 1, 2) {
	if (isPermanent) {
		m_state = permanent;
		setVisible(false);
		m_isIcemanPickupable = true;
	}
	else {
		m_state = temporary;
		setVisible(true);
		m_isIcemanPickupable = false;
	}
	m_timer = 0;

}

void Gold::doSomething() {
	if (!getAlive())
		return;
	switch (m_state)
	{
	case Gold::permanent:
		if (isRevealItself())
			return;
		else if (isPickup()) {
			GameController::getInstance().playSound(SOUND_GOT_GOODIE);
			getWorld()->getPlayer()->increGoldNum();
			getWorld()->increaseScore(10);
		}
		break;
	case Gold::temporary:
		if (m_timer > 99)
			setVisible(false);
		m_timer++;
		break;
	default:
		break;
	}

}

Gold::~Gold() {}


Oil::Oil(int startX, int startY)
	: ActivatingObject(IID_BARREL, startX, startY, right, 1, 2) {

}

void Oil::doSomething() {
	if (!getAlive())
		return;
	else if (isRevealItself())
		return;
	else  if (isPickup()) {
		GameController::getInstance().playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->decOilNum();
	}
	else
		return;
	
}

Oil::~Oil() {}


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
	int m_squirtBornX = 0;
	int m_squirtBornY = 0;
	
}

void Iceman::doSomething() {

	if (!getAlive())
		return;



	int i;
	if (getWorld()->getKey(i)) {
		switch (i)
		{
		case KEY_PRESS_ESCAPE:
			setAlive(false);
			break;
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
			if (m_waterAmmo <= 0)
				break;
			GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);
			m_waterAmmo--;
			setSquirtBornXY();
			getWorld()->shootWaterSquirt(GetSquirtBornX(), GetSquirtBornY());
		case KEY_PRESS_TAB:
			if (m_goldNuggest > 0) {
				getWorld()->dropGold(getX(), getY());
				m_goldNuggest--;
			}
			break;

		default:
			break;
		}
	}

	if (getWorld()->isCoveredByIce(getX(), getY())) {
		//m_needRemoveIce = true;
		getWorld()->removeIce(getX(), getY());
		GameController::getInstance().playSound(SOUND_DIG);
	}


	
}


void Iceman::getAnnoyed() { return; }
void Iceman::setSquirtBornXY() {
	Direction Dir = getDirection();
	switch (Dir) {
	case up:
		m_squirtBornX = getX();
		m_squirtBornY = getY() + 4;
		break;
	case down:
		m_squirtBornX = getX();
		m_squirtBornY = getY() - 4;
		break;
	case left:
		m_squirtBornX = getX() - 4;
		m_squirtBornY = getY();
		break;
	case right:
		m_squirtBornX = getX() + 4;
		m_squirtBornY = getY();
		break;
	default:
		break;
	}
}





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
	m_distTraveled = 0;
}



void Squirt::doSomething() {

	int squirtX = getX();
	int squirtY = getY();
	if (getWorld()->isCoveredByIce(squirtX, squirtY) || getWorld()->isBoulderAhead(squirtX, squirtY))
		setAlive(false);

	if (m_distTraveled <= 4) {
		switch (getDirection()) {

		case up:
			moveTo(squirtX, squirtY + 1);
			break;
		case down:
			moveTo(squirtX, squirtY- 1);
			break;
		case right:
			moveTo(squirtX + 1, squirtY);
			break;
		case left:
			moveTo(squirtX - 1, squirtY);
			break;
		default:
			break;
		}
		m_distTraveled++;
	}
	else {
		setAlive(false);
	}

}

Squirt::~Squirt() {}
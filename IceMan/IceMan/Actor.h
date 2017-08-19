#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth);
	virtual ~Actor();
	virtual void doSomething() { return; }
	virtual void getAnnoyed() { return; }
	StudentWorld* getWorld() { return m_studentWorld; }
	void setWorld(StudentWorld* studentWorld) { this->m_studentWorld = studentWorld; }
	void setIsAnnoyed(bool annoyedOrNot) { isAnnoyed = annoyedOrNot; }
	bool getIsAnnoyed() const { return isAnnoyed; } 
	void setAlive(bool aliveOrNot) { isAlive = aliveOrNot; }
	bool getAlive() const { return isAlive; } 
	bool isProtestor() const { return m_isProtestor; }
	void setProestor() { m_isProtestor = true; }
	virtual void KILLIT() {}

private:
	StudentWorld* m_studentWorld;
	bool isAlive;
	bool isAnnoyed;
	bool m_isProtestor;
};

class Agent : public Actor {
public:
	Agent(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth);
	~Agent();
	virtual void doSomething();
	enum ProtestorState { LeaveField, StayField, Rest };
	int getHitPoints() const { return m_hitPoints; }
	void setHitpoints(int hitPoints) { m_hitPoints = hitPoints; }
	int getTickCounter() const { return m_tickCounter; }
	void resetCounter() { m_tickCounter = 0; }
	void increCounter() { m_tickCounter++; }
	int getTicksToWait() const { return m_ticksToWaitBetweenMoves; }
	ProtestorState getProtestorState() const { return m_protestorState; }
	void increShoutCounter() { m_shoutCounter++; }
	void resetShoutCounter() { m_shoutCounter = 0; }
	int getShoutCounter() const { return m_shoutCounter; }
	void leaveField();
	virtual void getAnnoyed();
	void setNumSquare();
	int getNumSquare() const { return numSquaresToMoveInCurrentDirection; }
	void decreNumSqure() { numSquaresToMoveInCurrentDirection--; }
	void decreHitPoints() { m_hitPoints--; }
	void setTicksToWait();
	void setState(ProtestorState State) { m_protestorState = State; }
	void setRestTicks();
	int getRestTicks() const { return m_restTicks; }
	void resetRestCounter() { m_restCounter = 0; }
	void increRestCounter() { m_restCounter++; }
	int getRestCounter() const {return m_restCounter; }

private:
	int m_hitPoints;
	int numSquaresToMoveInCurrentDirection;
	ProtestorState m_protestorState;
	int m_tickCounter;
	int m_ticksToWaitBetweenMoves;
	int m_cuurentLevel;
	int getRandX();
	int m_shoutCounter;
	int m_restTicks;
	int m_restCounter;

};

class HardCoreProtestor : public Agent {
public:
	HardCoreProtestor();
	~HardCoreProtestor();
};

class RegularProtestor : public Agent {
public:
	RegularProtestor();
	~RegularProtestor();
};

class ActivatingObject : public Actor {
public:
	ActivatingObject(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth);
	~ActivatingObject();
	bool getActive() const { return isActive; }
	void setActive(bool isActiveOrNot) { isActive = isActiveOrNot; }
	bool isRevealItself();
	bool isPickup();
	enum State { permanent, temporary };
	bool switchState(int score, int lastTime, bool canPlayerPickUp);
	void setState(State targetState);
	int getTimer() const { return m_timer; }
	void increTimer() { m_timer++; }
	State getState() const { return m_state; }
private:
	bool isActive;
	State m_state;
	bool m_isIcemanPickupable;
	int m_timer;



};

class Oil : public ActivatingObject {
public:
	Oil(int startX, int startY);
	~Oil();
	virtual void doSomething();
private:


};

class Ice : public Actor {
public:
	Ice (int startX, int startY);
	~Ice();
};


class Iceman :public Actor {
public:
	Iceman();
	~Iceman();
	virtual void doSomething();
	virtual void getAnnoyed();
	int GetWaterAmount() const { return m_waterAmmo; } 
	int GetHealth() const { return m_hitPoints * 10; }
	int GetGold() const { return m_goldNuggest; }
	int GetSonar() const { return m_sonarCharge; }
	int GetSquirtBornX() const { return m_squirtBornX; }
	int GetSquirtBornY() const { return m_squirtBornY; }
	void setSquirtBornXY();
	void increGoldNum() { m_goldNuggest++; }
	void decreGoldNum() { m_goldNuggest--; }
	void increSonarKit() { m_sonarCharge++; }
	void decreSonarKit() { m_sonarCharge--; }
	void increWaterAmmo() { m_waterAmmo += 5; }
	void decreHitPoints() { m_hitPoints-=2; }
	
private:
	int m_hitPoints;
	int m_waterAmmo;
	int m_sonarCharge;
	int m_goldNuggest;
	int m_squirtBornX;
	int m_squirtBornY;
};


class Boulder 
	: public Actor {

public:
	Boulder(int startX, int startY);
	~Boulder();
	virtual void doSomething();
	

private:
	enum State { stable, waiting, falling };
	void checkState(State &state);
	bool isWaiting();
	bool isStopFalling();
	int m_tickCounter;
	int m_waitStart;
	State m_state;


};



class Squirt 
	:public Actor  {
public:
	Squirt(int startX, int startY, Direction startDir);
	~Squirt();
	virtual void doSomething();

private:
	int m_distTraveled;

};

class Gold : public ActivatingObject {
public:
	Gold(int startX, int startY, bool isPermanent);
	~Gold();
	virtual void doSomething();
};


class SonarKit : public ActivatingObject {
public:
	SonarKit(int startX, int startY);
	~SonarKit();
	virtual void doSomething();

};

class WaterPool : public ActivatingObject {
public:
	WaterPool(int startX, int startY);
	~WaterPool();
	virtual void doSomething();

};





#endif // ACTOR_H_

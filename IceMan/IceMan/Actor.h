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


private:
	StudentWorld* m_studentWorld;
	bool isAlive;
	bool isAnnoyed;

};

class ActivatingObject : public Actor {
public:
	ActivatingObject(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth);
	~ActivatingObject();
	bool getActive() const { return isActive; }
	void setActive(bool isActiveOrNot) { isActive = isActiveOrNot; }
	void RevealItself();
	bool isPickup();
private:
	bool isActive;



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
	bool isRemoveIce() { return m_needRemoveIce; }
	int GetWaterAmount() const { return m_waterAmmo; } 
	int GetHealth() const { return m_hitPoints * 10; }
	int GetGold() const { return m_goldNuggest; }
	int GetSonar() const { return m_sonarCharge; }
	bool isShoot() const { return m_isShoot; }
	int GetSquirtBornX() const { return m_squirtBornX; }
	int GetSquirtBornY() const { return m_squirtBornY; }
	void setSquirtBornXY();
	
private:
	int m_hitPoints;
	int m_waterAmmo;
	int m_sonarCharge;
	int m_goldNuggest;
	bool m_needRemoveIce;
	bool m_isShoot;
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







#endif // ACTOR_H_

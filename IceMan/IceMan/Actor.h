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
	bool getIsAnnoyed() { return isAnnoyed; }

	void setAlive(bool aliveOrNot) { isAlive = aliveOrNot; }
	bool getAlive() { return isAlive; }


private:
	StudentWorld* m_studentWorld;
	bool isAlive;
	bool isAnnoyed;

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


private:
	int m_hitPoints;
	int m_waterAmmo;
	int m_sonarCharge;
	int m_goldNuggest;
	bool m_needRemoveIce;
	bool m_isShoot;
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

private:

};







#endif // ACTOR_H_

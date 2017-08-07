#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth);
	//~Actor();
	virtual void doSomething()=0;
	StudentWorld* getWorld() {
		return m_studentWorld;
	}


private:
	StudentWorld* m_studentWorld;

};



class Ice : public Actor {
public:
	Ice (int startX, int startY);
	void doSomething();
	//~Ice();

private:

};


class Iceman :public Actor {
public:
	Iceman();
	//~Iceman();
	void doSomething();
	bool isRemoveIce() {
		return m_needRemoveIce;
	}


private:
	int m_hitPoints;
	int m_waterAmmo;
	int m_sonarCharge;
	int m_goldNuggest;
	bool m_needRemoveIce;
	

};









#endif // ACTOR_H_

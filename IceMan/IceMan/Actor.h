#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject {
public:
	Actor(int imageID, int startX, int startY, Direction startDirection, double size, unsigned int depth);
	~Actor();
	virtual void doSomething();
	


private:



};


class Ice : public Actor {
public:
	Ice ();
	~Ice();

private:

};



class Iceman :public Actor {
public:
	Iceman();
	~Iceman();
	void doSomething();

private:

	StudentWorld* m_StudentWorld;
	Ice* m_Ice;
	int m_hitPoints;
	int m_waterAmmo;
	int m_sonarCharge;
	int m_goldNuggest;
	


};









#endif // ACTOR_H_

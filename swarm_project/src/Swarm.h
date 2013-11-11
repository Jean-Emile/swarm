#ifndef SWARM_H_
#define SWARM_H_

#include "Mavlink.h"
#include "utils/Runnable.h"
#include <unordered_map>
#include "Repulsive.h"


class Swarm  : public Observateur,public Runnable
{
public:
	Swarm(Mavlink *mavlink);
	virtual ~Swarm();
	Repulsive repulsive;
	unordered_map<int,Drone*> drones;
	
	Drone * get(int id);
	void Update(const Observable* observable,mavlink_message_t msg);
	
protected:
	 void run();

	Mavlink *mavlink;
	
};

#endif /*SWARM_H_*/

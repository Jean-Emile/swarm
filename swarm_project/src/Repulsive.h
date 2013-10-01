#ifndef VELOCITY_H_
#define VELOCITY_H_

#include "utils/Vector3.h"
#include "Drone.h"
#include "Geopoint.h"

#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;

class Repulsive
{
public:
	Repulsive();
	virtual ~Repulsive();
    float getlat();
    float getlon();
    float getalt();
    

    unordered_map<int,Drone>  execute(unordered_map<int,Drone> drones);
	Vector3 gradient;
	float V;

	
private:

	unordered_map<int,Drone> drones;
	Geopoint compute_new_position(Geopoint& pos1, Geopoint& pos2,float distance);
	
};

#endif /*VELOCITY_H_*/

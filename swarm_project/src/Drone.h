#ifndef DRONE_H_
#define DRONE_H_
#include "Geopoint.h"

class Drone
{
public:
	Drone();
	Drone(int _sysID );
	Drone(int sysID,Geopoint position);
	virtual ~Drone();
	Drone* clone();
	int sysID;

	Geopoint getPosition();
    void update_position(Geopoint position);
    Geopoint position;
    

    double getDistance(Drone drone);
private:
  
   
};

#endif /*DRONE_H_*/

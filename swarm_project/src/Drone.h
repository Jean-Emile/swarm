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
	void heartbeat();

	Geopoint getPosition();
    void update_position(Geopoint &position);
    Geopoint position;
    void setAltitude(float altitude);
    float getBaroAltitude();
    

    double getDistance(Drone drone);
private:
  float altitude;
   
};

#endif /*DRONE_H_*/

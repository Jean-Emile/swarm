#include "Mavlink.h"

#include "Geopoint.h"
#include "Drone.h"
#include <unordered_map>
#include <string>
#include <iostream>

#include "Repulsive.h"
#include "Scheduler.h"
#include <vector>  
#include <thread>
#include "Swarm.h"





int main(int argc, char **argv) {

	Mavlink *mavlink = new Mavlink();

	mavlink->open("/dev/ttyACM0",115200);

	Swarm *swarm = new Swarm();
	swarm->AddObs(mavlink);
	
	mavlink->wait();
	

//	mavlink->flyhere()

	Repulsive *repulsive = new Repulsive();
	unordered_map<int,Drone> drones;

	Geopoint pos1(48.112843,-1.639478,10);
	Geopoint pos2(48.112854,-1.639431,10);
	Geopoint pos3(48.512854,-1.689431,10);

	Drone drone1(1,pos1);
	Drone drone2(2,pos2);
	Drone drone3(3,pos3);


	drones[1]= drone1;
	drones[2]= drone2;
	drones[3]= drone3;

	for (auto entry : drones)
	{ 
		printf("%f %f %f \n",entry.second.position.getlat(),entry.second.position.getlon(),entry.second.position.getalt());
	}

	printf("\n");

	unordered_map<int,Drone> result = repulsive->execute(drones);

	// todo plan

	for (auto entry : result)
	{ 
		printf("result -> %f %f %f \n",entry.second.position.getlat(),entry.second.position.getlon(),entry.second.position.getalt());
	}



	//	   printf("Distance between drone1 %f \n",drone1.getDistance(drone2));

	//	


	

	/*
	    Vector3 pos_update = grad_dist(*pos1,*pos2,5,25,0.01);


	    Vector3 newpos1   = *pos1 + pos_update;

	   // printf("Current 3D vector at (%f,%f,%f)\n",pos1->X,pos1->Y,pos1->Z);
	    printf("Target  3D vector at (%f,%f,%f)\n",pos_update.X,pos_update.Y,pos_update.Z);
	 */
	
	return 0;
}
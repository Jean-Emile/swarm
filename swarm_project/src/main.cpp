#include "Mavlink.h"


#include "Drone.h"
#include <unordered_map>
#include <string>
#include <iostream>

#include "Repulsive.h"
#include "Scheduler.h"
#include <vector>
#include <thread>
#include "Swarm.h"
#include "gui/Gui.h"

#include <gtk/gtk.h>




/*
void start()
{

	Repulsive *repulsive = new Repulsive();
	unordered_map<int,Drone> drones;


	Geopoint pos3(48.512854,-1.689431,10);


	std::list<Geopoint> resultcircle = pos3.createCircle(100,30);

	for (std::list<Geopoint>::const_iterator iterator = resultcircle.begin(), end = resultcircle.end(); iterator != end; ++iterator) {
		Geopoint r = *iterator;
		  printf("%f %f \n",r.getlat(),r.getlon());
		gui->addPoint(r);
	  sleep(1);
	}


}
*/

int main(int argc, char **argv) {


Mavlink	*mavlink = new Mavlink();

Swarm *swarm = new Swarm(mavlink);

	Gui	*gui = new Gui(argc, argv,mavlink,swarm);

gui->start();


/*
	




	mavlink->wait();
	*/

//	mavlink->flyhere()




	/*
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

*/

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

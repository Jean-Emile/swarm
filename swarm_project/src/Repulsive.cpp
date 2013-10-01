#include "Repulsive.h"
#include <math.h>
#include <stdlib.h>

Repulsive::Repulsive()
{
	V=0;
	gradient.X =0;
	gradient.Y=0;
	gradient.Z=0;

}

Repulsive::~Repulsive()
{
}



float Repulsive::getlat(){
	return gradient.X;

}
float Repulsive::getlon(){
	return gradient.Y;
}
float Repulsive::getalt(){
	return gradient.Z;
}





/**
 *  pos1  and pos2  are the 3D positions of quadcopter1 and quadcopter2 
(so pos1 is a 3-vector with 3 components x, y, z, and the same pos2)
Repulsive
- d_min is the minimum distance (a scalar) you want between the two 
quadrcopters

- d_max is the maximum distance (a scalar) after which no action should 
be taken

- gain is just a gain to tune the magnitude of the repulsive force. It 
should be small, like gain = 0.01 (but it depends a lot on what you 
choose for d_min and d_max)


- the resulting variable "gradient" is a 3-vector which represents the 
velocity you should add to quadcopter 1
 */

/*#include "mavlink_serial.h"
#include <iostream>
#include "Geopoint.h"
using namespace std;
#include <complex>      // std::complex, std::norm


#include "utils/Vector3.h"


// Converts radians to degrees
float Rad2Deg(float radians){
    return radians*(180/3.141592653589793238);
}

const double pi  =3.141592653589793238462;


Vector3 grad_dist(Vector3& pos1, Vector3& pos2,float  d_min, float d_max, float gain){

	float dist = pos1.DistanceTo(&pos2);
	float p1=(pi/2)/(d_max-d_min);
	float p2=-p1*d_max;
    float V=0;
    Vector3 gradient;
    printf("%f",dist);
	if (dist<=d_max){
	     V = gain*tan(p1*dist+p2)*tan(p1*dist+p2);
	     printf("V=%f\n",V);
	     float res = -2*gain*p1*tan(p1*dist+p2)/(pow(cos(p1*dist+p2),2));  
	     gradient = ((pos1 - pos2)*res)/dist;
	    // gradient = -2*gain*p1*tan(p1*dist+p2)/(cos(p1*dist+p2)^2)*(pos1-pos2)/dist;
printf("RESULT %f %f %f \n",gradient.X,gradient.Y,gradient.Z);
	     return gradient;
	}
	else{

	     V = 0;
	    // gradient = [0;0;0];
	     return gradient;
	}    

}


int main(int argc, char **argv) {





	    Vector3* pos1 = new Vector3(48.11538,-1.639227,10);


	    Vector3* pos2 = new Vector3(48.115366,-1.639224 ,10 );



	    grad_dist(*pos1,*pos2,5,10,0.1);



	    // Calculate spherical coordinates for the 3D vector






	return 0;
}*/

long map2(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double meters2degrees_lat(double meters) {
	/* polar radius = 6,356,752.3142m
	// so, one degree = 2*PI*radius/360 degrees  =appx 110,946m */
	return meters / (2.0*M_PI*6356752.3142/360.0);
}
double meters2degrees_lon(double meters) { /* convert meters longitude to degrees
	// equatorial radius = 6,378,137m
	// so, one degree = 2*PI*radius/360 degrees  =appx 111,319m */
	return (double)((meters / (2.0*M_PI*6378137.0/360.0)));
}

double degrees_lat2meters(double degrees) { /* convert latitude degrees to meters
	// polar radius = 6,356,752.3142m
	// so, one degree = 2*PI*radius/360 degrees  =appx 110,946m */
	return degrees * (2.0*M_PI*6356752.3142/360.0);
}
double degrees_lon2meters(double degrees) { /* convert meters longitude to degrees
	// equatorial radius = 6,378,137m
	// so, one degree = 2*PI*radius/360 degrees  =appx 111,319m */
	return degrees * (2.0*M_PI*6378137.0/360.0);
}

double feet2meters(double feet) {
	return feet * 0.3048006096;
}
double meters2feet(double meters) {
	return meters / 0.3048006096;
}

double deg2rad2(double degrees) { /* convert degrees to radians */
	return degrees * (M_PI/180.0);
}

double rad2deg2(double radians) { /* convert radian to degree */
	return radians * (180.0/M_PI);
}

int getangle(Geopoint& src, Geopoint& dest)
{
	//if(src.mode >= MODE_2D && dest.mode >= MODE_2D)
	//	{TO FIX
	double width, height;
	double bearing = -1;
	width  = fabs(src.getlon()- dest.getlon());
	height = fabs(src.getlat() - dest.getlat());

	if (dest.getlat() == src.getlat() && dest.getlon() == src.getlon())
	{
		return 0;
	} else if (dest.getlat() > src.getlat() && dest.getlon() == src.getlon()) {
		bearing = 0;
	} else if (dest.getlat() == src.getlat() && dest.getlon() > src.getlon()) {
		bearing = 90;
	} else if (dest.getlat() < src.getlat() && dest.getlon() == src.getlon()) {
		bearing = 180;
	} else if (dest.getlat() == src.getlat() && dest.getlon() < src.getlon()) {
		bearing = 270;
	} else if (dest.getlat() > src.getlat() && dest.getlon() > src.getlon()) {
		bearing = rad2deg2(atan(width/height));
	} else if (dest.getlat() < src.getlat() && dest.getlon() > src.getlon()) {
		bearing = rad2deg2(atan(height/width)) + 90;
	} else if (dest.getlat() < src.getlat() && dest.getlon() < src.getlon()) {
		bearing = rad2deg2(atan(width/height)) + 180;
	} else if (dest.getlat() > src.getlat() && dest.getlon() < src.getlon()){
		bearing = rad2deg2(atan(height/width)) + 270;
	}

	return bearing;
	/*
	}else
	{

		return -1;
	}*/

}

Geopoint Repulsive::compute_new_position(Geopoint& pos1, Geopoint& pos2,float distance){
		double width, height;
		int heading =	(getangle(pos1,pos2)+180)%360;
		/* calculate new location */
		if (heading < 90) {
			width  =  distance * sin(deg2rad2(heading));
			height =  distance * cos(deg2rad2(heading));
		} else if (heading< 180) {
			width  =  distance * cos(deg2rad2(heading-90));
			height = -distance * sin(deg2rad2(heading-90));
		} else if (heading < 270) {
			width  = -distance * sin(deg2rad2(heading-180));
			height = -distance * cos(deg2rad2(heading-180));
		} else {
			width  = -distance * cos(deg2rad2(heading-270));
			height =  distance * sin(deg2rad2(heading-270));
		}
		//printf("heading %d current %f distance %f \n",heading,dist,distance);
		pos1.setlat(pos1.getlat()+ meters2degrees_lon(width));
		pos1.setlon(pos1.getlon()+ meters2degrees_lat(height));
	return pos1;
}

unordered_map<int,Drone>  Repulsive::execute(unordered_map<int,Drone> drones){

	int i=0;
	unordered_map<int,Drone> drones_updated;
	float gain = 0;
	float d_min = 5;
	float d_max = 15;

	for (auto entry : drones)
	{
		Drone current = entry.second;
		int id = entry.second.sysID;

		for (auto entry2 : drones)
		{
			if(id != entry2.second.sysID)
			{		
				//printf("Repulsive %d - %d \n ",id,entry2.first);
				Drone target = entry2.second;
				float dist = current.getDistance(target);
				double width, height,distance;
				distance =fabs(dist-d_min);
				if (dist<=d_max)
				{
					Geopoint p1 = current.getPosition();
					Geopoint p2 = target.getPosition();
					Drone clone = *current.clone();
					clone.position  = compute_new_position(p1,p2,dist);
					drones_updated[i] = clone;
					i++;
				}
			}

		}	
	}
	return drones_updated;
}

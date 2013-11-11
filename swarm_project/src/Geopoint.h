#ifndef GEOPOINT_H_
#define GEOPOINT_H_

#include "utils/Vector3.h"
#include <list>
#include <string>


#define MODE_NOT_SEEN	0	/* mode update not seen yet */
#define MODE_NO_FIX	1	/* none */
#define MODE_2D  	2	/* good for latitude/longitude */
#define MODE_3D  	3	/* good for altitude/climb too */

class Geopoint
{
public:
	Geopoint();
	virtual ~Geopoint();
	Geopoint(float X, float Y, float Z);
    float getlat();
    float getlon();
    float getalt();
    void setlat(float lat);
    void setlon(float lon);
    void setalt(float lon);
    std::string toString();
    Vector3 getvector();
    double getDistance(Geopoint point);
    std::list<Geopoint> createCircle(int count,double circleRadius);

	int    mode;	/* Mode of fix */
private:

	Vector3 vector;
	
	/* Latitude in degrees (valid if mode >= 2) */
	/* Longitude in degrees (valid if mode >= 2) */
	/* Altitude in meters (valid if mode == 3) */

	float track;	/* Course made good (relative to true north) */
	double online;		/* NZ if GPS is on line, 0 if not. */
	float speed;	/* Speed over ground, km/h */
	int satellites_used;

	double eps;		// Speed uncertainty, meters/sec
	double climb;  //  Vertical speed, meters/sec
	double epc;		// Vertical speed uncertainty
	float eph;  	 // Horizontal position uncertainty, meters
	float ept;		//	 Expected time uncertainty
	double time;	//	 Time of update, seconds since Unix epoch
	float epd;		// Track uncertainty, degrees

};

#endif /*GEOPOINT_H_*/

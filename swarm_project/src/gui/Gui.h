#ifndef GUI_H_
#define GUI_H_



#include "../Geopoint.h"
#include <osm-gps-map-compat.h>
#include <osm-gps-map-source.h>
#include <osm-gps-map-osd.h>

#define UI_FILE "gui/gui.glade"


#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <functional>
#include <list>
#include <GL/glut.h>
#include "../Mavlink.h"
#include "../Swarm.h"



	
class Gui : public Observateur
{
public:
	Gui(){}
	Gui(int argc, char **argv,Mavlink *mavlink,Swarm *swarm);
	~Gui();
	void start();
	void addPoint(Geopoint p);
	const char *repo_uri;
	char *cachedir, *cachebasedir;
	GOptionContext *context;
	GtkWidget  *boxmap;
	GdkPixbuf *copter;
	OsmGpsMapSource_t map_provider;
	OsmGpsMap *map;
	OsmGpsMapLayer *osd;


		    

};

#endif /*GUI_H_*/

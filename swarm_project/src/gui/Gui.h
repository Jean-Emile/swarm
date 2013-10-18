#ifndef GUI_H_
#define GUI_H_



#include <osm-gps-map-compat.h>
#include <osm-gps-map-source.h>
#include <osm-gps-map-osd.h>
#define UI_FILE "gui/gui.glade"


class Gui
{
public:
	Gui();
	virtual ~Gui();
	void start();
	void addPoint(double lat,double lon);

	const char *repo_uri;
	char *cachedir, *cachebasedir;
	GOptionContext *context;
	GtkWidget  *boxmap;
	GdkPixbuf *copter;
	OsmGpsMapSource_t map_provider;
	OsmGpsMap *map;

};

#endif /*GUI_H_*/

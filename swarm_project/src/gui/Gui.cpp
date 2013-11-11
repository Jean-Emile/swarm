#include "Gui.h"
#include <thread>
#include <list>
using namespace std;


static GLfloat g_nearPlane = 1;
static GLfloat g_farPlane = 1000;

static int g_Width = 600;                          // Initial window width
static int g_Height = 600;                         // Initial window height


list<Geopoint> drones;

void runGUI(){
	gtk_main ();
}


void changeSize(int w, int h)
{

    // prevent divide-by-zero, when window height of zero
    if(h == 0) {
        h = 1;
    }
    float ratio = (1.0 * w) / h;

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glViewport(0, 0, w, h);
 //   gluPerspective(45, ratio, 1, 1060);

    glMatrixMode(GL_MODELVIEW);
}


void myReshape(GLint width, GLint height)
{
   g_Width = width;
   g_Height = height;
   glViewport(0, 0, g_Width, g_Height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(65.0, (float)g_Width / g_Height, g_nearPlane, g_farPlane);
   glMatrixMode(GL_MODELVIEW);
}






void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        list<Geopoint>::iterator i;
   float dToR = M_PI / 180.0;
   
   glPointSize(5.0);
	glColor3f(1.0,1.0,0.0);
	 
	   for(i=drones.begin(); i != drones.end(); ++i) {
 
    GLfloat x, y, z;
     /* WGS84 parameters. */
    int a = 6378137; // earth radius
	float f  = 1/298.257223563 ;   //flattening
	float b  = a*(1-f);
	float e2 = 1-pow((b/a),2);
            
     // convert latitude and longitude from degrees to radians
	float latitude = i->getlat() * M_PI/180;
	float longitude = i->getlon() * M_PI/180;
	
	float altitude  = i->getalt();
	                          //flattening
    /*Conversion from:
 en.wikipedia.org/wiki/Geodetic_system#Conversion_calculations */
    float Nphi =  a / sqrt(1 - e2*pow(sin(latitude),2));
	x = (Nphi + altitude)*cos(latitude)*cos(longitude);
	y = (Nphi + altitude)*cos(latitude)*sin(longitude);
	z = (Nphi*(1 - e2) + altitude)*sin(latitude);

               printf("%f %f %f \n",x,y,z);
       glBegin(GL_POINTS);            
         glVertex3d(1,1,1);
   glEnd();
    
    }
 
  glutSwapBuffers();
}

static gboolean
on_zoom_in_clicked_event (GtkWidget *widget, gpointer user_data)
{
    int zoom;
    OsmGpsMap *map = OSM_GPS_MAP(user_data);
    g_object_get(map, "zoom", &zoom, NULL);
    osm_gps_map_set_zoom(map, zoom+1);
    return FALSE;
}


void earth()
{
double x, y, z, thet, phi;
int nlat, nlong;
double c;
x=y=0;
z=1;
glBegin(GL_TRIANGLE_FAN);
glVertex3d(x,y,z);
c=3.14159/180.0;
z=sin(c*80.0);
for(thet=-180.0; thet<=180.0;thet+=20.0)
{
x=sin(c*thet)*cos(c*80.0);
y=cos(c*thet)*cos(c*80.0);
glVertex3d(x,y,z);
}
glEnd();
x=y=0;
z=-1;
glBegin(GL_TRIANGLE_FAN);
glVertex3d(x,y,z);
z=-sin(c*80.0);
for(thet=-180.0; thet<=180.0;thet+=20.0)
{
x=sin(c*thet)*cos(c*80.0);
y=cos(c*thet)*cos(c*80.0);
glVertex3d(x,y,z);
}
glEnd();
for(phi=-80.0; phi<=80.0; phi+=20.0)
{
glBegin(GL_QUAD_STRIP);
for(thet=-180.0; thet<=180.0;thet+=20.0)
{
x=sin(c*thet)*cos(c*phi);
y=cos(c*thet)*cos(c*phi);
z=sin(c*phi);
glVertex3d(x,y,z);
x=sin(c*thet)*cos(c*(phi+20.0));
y=cos(c*thet)*cos(c*(phi+20.0));
z=sin(c*(phi+20.0));
glVertex3d(x,y,z);
}
glEnd();
}
}

void connect_mavlink(GtkButton * button,Mavlink *mavlink)
{
	cout << "connect" << endl;
	mavlink->open("/dev/ttyUSB0",57600);
}

void disconnect_mavlink(GtkButton * button,Mavlink *mavlink)
{
	cout << "connect" << endl;
	mavlink->shutdown();
}

void start_swarm(GtkButton * button,Swarm *swarm){
	swarm->start();
}


void stop_swarm(GtkButton * button,Swarm *swarm){
	swarm->stop();
}




Gui::Gui(int argc, char **argv,Mavlink *mavlink,Swarm *swarm)
{
	
	mavlink->AddObs(this);
	
	gtk_init (&argc, &argv);
	glutInit(&argc, argv);
	
	// GLUT

    glutInitWindowSize(g_Width, g_Height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Swarm 3D");

    // callbacks
   // glutDisplayFunc(renderScene);
    glutReshapeFunc(myReshape);
   // glutIdleFunc(renderScene);


	// GTK 
	GError *error = NULL;
	GtkWidget *main_window;

	GtkBuilder *builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE, &error))
	{
		g_warning ("Couldn't load builder file : %s", error->message);
		g_error_free (error);
	}
	main_window=    GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
	//g_signal_connect (G_OBJECT (main_window),"destroy", main_quit, NULL);
	gtk_window_set_title (GTK_WINDOW (main_window), "SWARM 1.0");
	gtk_window_set_position (GTK_WINDOW (main_window), GTK_WIN_POS_CENTER);
	boxmap = GTK_WIDGET (gtk_builder_get_object (builder, "map"));

	/* MAP */
	/*******************************************************************************************/

	map_provider = OSM_GPS_MAP_SOURCE_GOOGLE_STREET;
	map  = (OsmGpsMap *) g_object_new (OSM_TYPE_GPS_MAP,
			"map-source", map_provider,
			"tile-cache", "../cache_MAP/", "proxy-uri", g_getenv ("http_proxy"), NULL);


	/*Set the starting coordinates and zoom level for the map */
	osm_gps_map_set_zoom (map, 30);

	/* coordonnées GPS ménéac */
	osm_gps_map_set_center (map, 48.512856,-1.689363);
	gtk_box_pack_start (GTK_BOX (boxmap), GTK_WIDGET (map), TRUE, TRUE, 0);
    g_signal_connect ( gtk_builder_get_object(builder, "zoom_in_button"), "clicked",G_CALLBACK (on_zoom_in_clicked_event), (gpointer) map);
    
    GtkWidget * button_connect = GTK_WIDGET (gtk_builder_get_object (builder, "button_connect"));
	g_signal_connect (G_OBJECT (button_connect), "clicked", G_CALLBACK (connect_mavlink),mavlink);
	
	
	GtkWidget * button_start_swarm = GTK_WIDGET (gtk_builder_get_object (builder, "button_start_swarm"));
	g_signal_connect (G_OBJECT (button_start_swarm), "clicked", G_CALLBACK (start_swarm),swarm);
	
	
		
	GtkWidget * button_stop_swarm = GTK_WIDGET (gtk_builder_get_object (builder, "button_stop_swarm"));
	g_signal_connect (G_OBJECT (button_stop_swarm), "clicked", G_CALLBACK (stop_swarm),swarm);
	
	
	
	
	
	copter = gdk_pixbuf_new_from_file_at_size ("copter.png", 15,15,NULL);


 osd =(    OsmGpsMapLayer*) g_object_new (OSM_TYPE_GPS_MAP_OSD,
                        "show-scale",TRUE,
                        "show-coordinates",TRUE,
                       // "show-crosshair",TRUE,
                        "show-dpad",TRUE,
                        "show-zoom",TRUE,
                        "show-gps-in-dpad",TRUE,
                        "show-gps-in-zoom",FALSE,
                        "dpad-radius", 30,
                        NULL);
                        
                            osm_gps_map_layer_add(OSM_GPS_MAP(map), osd);

	gtk_widget_show(boxmap);
	gtk_widget_show((GtkWidget*)map);
	gtk_widget_show (main_window);


}


void display(void)
{
	
}
Gui::~Gui()
{
}

void Gui::addPoint(Geopoint p){
 osm_gps_map_image_add(map,p.getlat(),p.getlon(),copter);
osm_gps_map_set_center (map, p.getlat(),p.getlon());
 drones.clear();
   drones.push_back(p);
}
void Gui::start(){
	//std::thread t1(runGUI);
runGUI();
	  //  glutMainLoop();
}

#
#	
#	Makefile SWARM 

#
INCDIR=.

OSM_GPS_MAP=osm-gps-map


CC=g++
CCFLAGS= -I$(INCDIR)  `pkg-config --cflags gtk+-2.0` 
CCFLAGS+= -I/usr/include/libsoup-2.4 -I/usr/include/libxml2 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/osmgpsmap
LDFLAGS= -lpthread -lm -g -llog4c -std=c++0x -losmgpsmap


TARGET= main

FILES=	Drone.cpp \
	Geopoint.cpp \
	main.cpp \
	Mavlink.cpp \
	Repulsive.cpp \
	Scheduler.cpp\
	Swarm.cpp\
	utils/Observateur.cpp\
	utils/Vector3.cpp \
	gui/Gui.cpp \

	

OBJECTS=$(FILES:.c=.o)


#-------------  Appli  --------------
all: $(TARGET) 

#-------------  Appli  --------------


main : $(OBJECTS)
	$(CC) $(OBJECTS) $(CCFLAGS) \
		-o main $(LDFLAGS) ${OSM_MAP_OBJECTS}

exec:
	@(cd $(WIDGET_DIR) && $(MAKE))
	@(cd $(OSM_GPS_MAP) && $(MAKE))

#------------------------------------

.c.o :
	$(CC) $(CCFLAGS) -c $< \
		-o $*.o 

 
dep :
	@echo "======== Mise a jour des dependances : .depend ========"
	@rm -f .depend
	@for i in *.c ; do \
	echo $$i ; \
	$(CC) $(DEPOPT) $(CCFLAGS) $$i > .tmpdepend ; \
	OBJNAME=`echo $$i | sed -e s%\\\.c%.o% ` ; \
	cat .tmpdepend | \
	sed -e s%`basename $$i .c`\\\.o%$$OBJNAME%         >> .depend ; \
	echo  >> .depend ; \
	done
	@rm -f .tmpdepend	

 
CLEANING=rm -rf $(TARGET); 
CONSEIL=echo PoWet


clear :
	@$(CLEANING)
	@echo
	@$(CONSEIL)
	@echo
clean :
	$(CLEANING)
	@echo
	@$(CONSEIL)
	@echo
upload :
	./up.sh

sinclude .depend


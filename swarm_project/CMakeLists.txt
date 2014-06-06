cmake_minimum_required(VERSION 2.8)
project(swarm_project)


set(SRC_PATH src)
set(OSM_PATH /usr/include/osmgpsmap)

include_directories(${SRC_PATH})
include_directories(${SRC_PATH}/gui)
include_directories(${SRC_PATH}/mavlink)
include_directories(${SRC_PATH}/utils)


file(GLOB_RECURSE files ${SRC_PATH}/*)
ADD_DEFINITIONS("-std=c++0x ") 
ADD_DEFINITIONS("-I/usr/include/libsoup-2.4 -I/usr/include/libxml2 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/osmgpsmap") 
ADD_DEFINITIONS("-pthread -I/usr/include/gtk-2.0 -I/usr/lib/i386-linux-gnu/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/glib-2.0 -I/usr/lib/i386-linux-gnu/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12")




add_executable(
   	main
        ${files}
)
TARGET_LINK_LIBRARIES(main glut)
TARGET_LINK_LIBRARIES(main osmgpsmap)
TARGET_LINK_LIBRARIES(main GLU)
TARGET_LINK_LIBRARIES(main GL)




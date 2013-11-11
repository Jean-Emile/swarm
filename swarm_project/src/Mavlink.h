#ifndef MAVLINK_SERIAL_H_
#define MAVLINK_SERIAL_H_

// Standard includes
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <string.h>
#include <inttypes.h>
#include <fstream>
// Serial includes
#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#ifdef __linux
#include <sys/ioctl.h>
#endif

// Latency Benchmarking
#include <sys/time.h>
#include <time.h>
#include "mavlink/v1.0/common/mavlink.h"
#include "mavlink/v1.0/common/common.h"
#include <Geopoint.h>
#include <Drone.h>
#include <atomic>
#include <thread>
#include <utils/Observateur.h>
#include <utils/Runnable.h>

using namespace std;




class Mavlink : public Runnable,public Observable
{
public:
	Mavlink();
	virtual ~Mavlink();
	int open(const char *uart_name,int baudrate);
	int shutdown();
	void wait();
	void flyhere(Drone drone,Geopoint point);
	void arm(Drone drone);
	void disarm(Drone drone);
	void landing(Drone drone);
	void requestData(Drone drone);
	
	  /* Fonctions de création et destruction du singleton
  static Mavlink *getInstance ()
  {
    if (NULL == _singleton)
      {
        std::cout << "creating singleton." << std::endl;
        _singleton =  new Mavlink;
      }
    else
      {
        std::cout << "singleton already created!" << std::endl;
      }

    return _singleton;
  }

  static void kill ()
  {
    if (NULL != _singleton)
      {
        delete _singleton;
        _singleton = NULL;
      }
  }
*/

protected:
	void run();

private:
	int serial_fd;
	const char* uart_name;
	int baudrate;
	void restart();
	void resquestStream(Drone drone,int req_stream_id,int rate);
};








#endif 

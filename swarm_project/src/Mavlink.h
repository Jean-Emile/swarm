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
#include "Geopoint.h"
#include "Drone.h"
#include <thread>
#include "utils/Observateur.h"

using namespace std;

int serial_wait(int serial_fd);

class Mavlink : public Observable
{
public:
	Mavlink();
	virtual ~Mavlink();
	int open(const char *uart_name,int baudrate);
	int close();
	void wait();
	void flyhere(Drone drone,Geopoint point);
	void arm(Drone drone);
	void disarm(Drone drone);
	void landing(Drone drone);

private:
	thread task_serial;
	int serial_fd;
	
};








#endif 
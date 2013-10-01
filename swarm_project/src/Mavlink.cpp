#include "Mavlink.h"
#include <thread>



struct timeval tv;		  ///< System time

// Settings
int sysid = 42;             ///< The unique system id of this MAV, 0-127. Has to be consistent across the system
int compid = 110;
int serial_compid = 0;

bool silent = false;              ///< Wether console output should be enabled
bool verbose = false;             ///< Enable verbose output
bool debug = false;               ///< Enable debug functions and output


using namespace std;



Mavlink::Mavlink()
{
}

Mavlink::~Mavlink()
{
}





int Mavlink::close()
{

	return 0;
}

/**
 *
 *
 * Returns the file descriptor on success or -1 on error.
 */

int open_port(const char* port)
{
	int fd; /* File descriptor for the port */

	// Open serial port
	// O_RDWR - Read and write
	// O_NOCTTY - Ignore special chars like CTRL-C
	fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		/* Could not open the port. */
		return(-1);
	}
	else
	{
		fcntl(fd, F_SETFL, 0);
	}

	return (fd);
}

bool setup_port(int fd, int baud, int data_bits, int stop_bits, bool parity, bool hardware_control)
{
	//struct termios options;

	struct termios  config;
	if(!isatty(fd))
	{
		fprintf(stderr, "\nERROR: file descriptor %d is NOT a serial port\n", fd);
		return false;
	}
	if(tcgetattr(fd, &config) < 0)
	{
		fprintf(stderr, "\nERROR: could not read configuration of fd %d\n", fd);
		return false;
	}
	//
	// Input flags - Turn off input processing
	// convert break to null byte, no CR to NL translation,
	// no NL to CR translation, don't mark parity errors or breaks
	// no input parity check, don't strip high bit off,
	// no XON/XOFF software flow control
	//
	config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL |
			INLCR | PARMRK | INPCK | ISTRIP | IXON);
	//
	// Output flags - Turn off output processing
	// no CR to NL translation, no NL to CR-NL translation,
	// no NL to CR translation, no column 0 CR suppression,
	// no Ctrl-D suppression, no fill characters, no case mapping,
	// no local output processing
	//
	config.c_oflag &= ~(OCRNL | ONLCR | ONLRET |
			ONOCR | OFILL | OPOST);

#ifdef OLCUC 
	config.c_oflag &= ~OLCUC; 
#endif

#ifdef ONOEOT
	config.c_oflag &= ~ONOEOT;
#endif

	//
	// No line processing:
	// echo off, echo newline off, canonical mode off,
	// extended input processing off, signal chars off
	//
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
	//
	// Turn off character processing
	// clear current char size mask, no parity checking,
	// no output processing, force 8 bit input
	//
	config.c_cflag &= ~(CSIZE | PARENB);
	config.c_cflag |= CS8;
	//
	// One input byte is enough to return from read()
	// Inter-character timer off
	//
	config.c_cc[VMIN]  = 1;
	config.c_cc[VTIME] = 10; // was 0

	// Get the current options for the port
	//tcgetattr(fd, &options);

	switch (baud)
	{
	case 1200:
		if (cfsetispeed(&config, B1200) < 0 || cfsetospeed(&config, B1200) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
			return false;
		}
		break;
	case 1800:
		cfsetispeed(&config, B1800);
		cfsetospeed(&config, B1800);
		break;
	case 9600:
		cfsetispeed(&config, B9600);
		cfsetospeed(&config, B9600);
		break;
	case 19200:
		cfsetispeed(&config, B19200);
		cfsetospeed(&config, B19200);
		break;
	case 38400:
		if (cfsetispeed(&config, B38400) < 0 || cfsetospeed(&config, B38400) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
			return false;
		}
		break;
	case 57600:
		if (cfsetispeed(&config, B57600) < 0 || cfsetospeed(&config, B57600) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
			return false;
		}
		break;
	case 115200:
		if (cfsetispeed(&config, B115200) < 0 || cfsetospeed(&config, B115200) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
			return false;
		}
		break;

		// These two non-standard (by the 70'ties ) rates are fully supported on
		// current Debian and Mac OS versions (tested since 2010).
	case 460800:
		if (cfsetispeed(&config, 460800) < 0 || cfsetospeed(&config, 460800) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
			return false;
		}
		break;
	case 921600:
		if (cfsetispeed(&config, 921600) < 0 || cfsetospeed(&config, 921600) < 0)
		{
			fprintf(stderr, "\nERROR: Could not set desired baud rate of %d Baud\n", baud);
			return false;
		}
		break;
	default:
		fprintf(stderr, "ERROR: Desired baud rate %d could not be set, aborting.\n", baud);
		return false;

		break;
	}

	//
	// Finally, apply the configuration
	//
	if(tcsetattr(fd, TCSAFLUSH, &config) < 0)
	{
		fprintf(stderr, "\nERROR: could not set configuration of fd %d\n", fd);
		return false;
	}
	return true;
}

void close_port(int fd)
{
	close(fd);
}

/**
 * @brief Serial function
 *
 * This function blocks waiting for serial data in it's own thread
 * and forwards the data once received.
 */
int serial_wait(int serial_fd,const Mavlink  &mavlink)
{
	int fd = serial_fd;

	mavlink_status_t lastStatus;
	lastStatus.packet_rx_drop_count = 0;

	// Blocking wait for new data
	while (1)
	{
		//if (debug) printf("Checking for new data on serial port\n");
		// Block until data is available, read only one byte to be able to continue immediately
		//char buf[MAVLINK_MAX_PACKET_LEN];
		uint8_t cp;
		mavlink_message_t message;
		mavlink_status_t status;
		uint8_t msgReceived = false;

		if (read(fd, &cp, 1) > 0)
		{
			// Check if a message could be decoded, return the message in case yes
			msgReceived = mavlink_parse_char(MAVLINK_COMM_1, cp, &message, &status);
			if (lastStatus.packet_rx_drop_count != status.packet_rx_drop_count)
			{
				if (verbose || debug) printf("ERROR: DROPPED %d PACKETS\n", status.packet_rx_drop_count);
				if (debug)
				{
					unsigned char v=cp;
					fprintf(stderr,"%02x ", v);
				}
			}
			lastStatus = status;
		}
		else
		{
			if (!silent) fprintf(stderr, "ERROR: Could not read from fd %d\n", fd);
		}

		// If a message could be decoded, handle it
		if(msgReceived)
		{
			//if (verbose || debug) std::cout << std::dec << "Received and forwarded serial port message with id " << static_cast<unsigned int>(message.msgid) << " from system " << static_cast<int>(message.sysid) << std::endl;

			// Do not send images over serial port

			// DEBUG output
			if (debug)
			{
				fprintf(stderr,"Received serial data: ");
				unsigned int i;
				uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
				unsigned int messageLength = mavlink_msg_to_send_buffer(buffer, &message);
				if (messageLength > MAVLINK_MAX_PACKET_LEN)
				{
					fprintf(stderr, "\nFATAL ERROR: MESSAGE LENGTH IS LARGER THAN BUFFER SIZE\n");
				}
				else
				{
					for (i=0; i<messageLength; i++)
					{
						unsigned char v=buffer[i];
						fprintf(stderr,"%02x ", v);
					}
					fprintf(stderr,"\n");
				}
			}

			if (verbose || debug)
				printf("Received message from serial with ID #%d (sys:%d|comp:%d):\n", message.msgid, message.sysid, message.compid);

			/* decode and print */


			// For full MAVLink message documentation, look at:
			// https://pixhawk.ethz.ch/mavlink/

			// Only print every n-th message
			//static unsigned int scaled_imu_receive_counter = 0;

			//  Notify();
			switch (message.msgid)
			{

			case MAVLINK_MSG_ID_HEARTBEAT:
				mavlink_heartbeat_t inp;
				mavlink_msg_heartbeat_decode(&message, &inp);
				printf("HEAR BEAT mavlink_version %d \n",inp.mavlink_version);
				break;	

			case MAVLINK_MSG_ID_SYS_STATUS:
				mavlink_sys_status_t status;
				mavlink_msg_sys_status_decode(&message, &status);
				printf("SYS ID %d \n",message.sysid);
				break;



			case MAVLINK_MSG_ID_HIGHRES_IMU:
			{

				mavlink_highres_imu_t imu;
				mavlink_msg_highres_imu_decode(&message, &imu);

				printf("Got message HIGHRES_IMU (spec: https://pixhawk.ethz.ch/mavlink/#HIGHRES_IMU)\n");
				printf("\t time: %llu\n", imu.time_usec);
				printf("\t acc  (NED):\t% f\t% f\t% f (m/s^2)\n", imu.xacc, imu.yacc, imu.zacc);
				printf("\t gyro (NED):\t% f\t% f\t% f (rad/s)\n", imu.xgyro, imu.ygyro, imu.zgyro);
				printf("\t mag  (NED):\t% f\t% f\t% f (Ga)\n", imu.xmag, imu.ymag, imu.zmag);
				printf("\t baro: \t %f (mBar)\n", imu.abs_pressure);
				printf("\t altitude: \t %f (m)\n", imu.pressure_alt);
				printf("\t temperature: \t %f C\n", imu.temperature);
				printf("\n");


			}
			break;
			}
		}
	}
	return 0;
}

void Mavlink::wait(){
	task_serial.join();
}


void Mavlink::flyhere(Drone drone,Geopoint point){
	mavlink_message_t message;
	char buf[300];
	mavlink_mission_item_t item;
	item.seq = 0;
	item.target_system = drone.sysID;
	item.target_component = 0;
	item.frame = MAV_FRAME_MISSION;
	item.x = (float)point.getlat()/1e6;
	item.y = (float)point.getlon()/1e6;
	item.z = (float)point.getalt();
	item.command = MAV_CMD_NAV_WAYPOINT;

	item.autocontinue = 0;
	item.param1 = 0;
	item.param2 = 0; //milliseconds
	item.param3 = 0;
	item.param4 = 0;
	//current = 2 is a flag to tell us this is a "guided mode" waypoint and not for the mission
	item.current = 2; 
	//static inline uint16_t mavlink_msg_mission_item_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mission_item_t* mission_item)

	mavlink_msg_mission_item_encode(drone.sysID,0,&message,&item);

	unsigned len = mavlink_msg_to_send_buffer((uint8_t*)buf, &message);

	/* write packet via serial link */
	write(serial_fd, buf, len);
	/* wait until all data has been written */
	tcdrain(serial_fd);
}


void Mavlink::arm(Drone drone){
	// todo
}
void Mavlink::disarm(Drone drone){
	// todo
}
void Mavlink::landing(Drone drone){
	// todo
}



int Mavlink::open(const char *uart_name,int baudrate){

	printf("Trying to connect to %s.. \n", uart_name);

	int fd = open_port(uart_name);

	if (fd == -1)
	{
		printf("failure, could not open port.\n");
		return -1;
	}
	else
	{
		printf("success.\n");
	}
	printf("Trying to configure %s.. ", uart_name);
	bool setup = setup_port(fd, baudrate, 8, 1, false, false);
	if (!setup)
	{
		printf("failure, could not configure port.\n");
		return -1;
	}
	else
	{
		printf("success.\n");
	}

	int noErrors = 0;
	if (fd == -1 || fd == 0)
	{
		fprintf(stderr, "Connection attempt to port %s with %d baud, 8N1 failed, exiting.\n", uart_name, baudrate);
		exit(EXIT_FAILURE);
	}
	else
	{
		fprintf(stderr, "\nConnected to %s with %d baud, 8 data bits, no parity, 1 stop bit (8N1)\n", uart_name, baudrate);
	}

	if(fd < 0)
	{
		exit(noErrors);
	}

	// Run indefinitely while the serial loop handles data
	printf("\nREADY, waiting for serial data. %d \n",fd);
	serial_fd = fd;

	task_serial = thread(serial_wait,fd,&this);


	return 0;
}







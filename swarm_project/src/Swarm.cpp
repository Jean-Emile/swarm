#include "Swarm.h"

Swarm::Swarm(Mavlink *mavlink)
{
mavlink->AddObs(this);
this->mavlink = mavlink;
}

Swarm::~Swarm()
{
}

Drone * Swarm::get(int sysid)
{
				if(drones.find(sysid) !=drones.end()){
					// found
					//	cout << "udate Drone" <<  sysid << endl;
					return drones[sysid];
				}else {
					// not found
					//cout << "new Drone detected" << sysid <<  endl;
					Drone *drone = new Drone(sysid);
					drones[sysid] = drone;
					mavlink->requestData(*drone);
					return drones[sysid];
					
				}	
}	
void Swarm::Update(const Observable* observable,mavlink_message_t message)
{

/*
 * 	uint16_t checksum; /// sent at end of packet
	uint8_t magic;   ///< protocol magic marker
	uint8_t len;     ///< Length of payload
	uint8_t seq;     ///< Sequence of packet
	uint8_t sysid;   ///< ID of message sender system/aircraft
	uint8_t compid;  ///< ID of the message sender component
	uint8_t msgid;   ///< ID of message in payload*/
	     	Drone *current = get(message.sysid);
			switch (message.msgid)
			{

				case MAVLINK_MSG_ID_HEARTBEAT:
				mavlink_heartbeat_t inp;
				mavlink_msg_heartbeat_decode(&message, &inp);
				current->heartbeat();
			
				
				break;	
				
				case MAVLINK_MSG_ID_SYS_STATUS:
				mavlink_sys_status_t status;
				mavlink_msg_sys_status_decode(&message, &status);
			//	printf("SYS ID %d \n",message.sysid);
				break;
				
				case MAVLINK_MSG_ID_GPS_STATUS:
					
				cout << "GPS STATUS " << endl;
				//current->update_position();
				break;
				
				
				case MAVLINK_MSG_ID_VFR_HUD:
					mavlink_vfr_hud_t hud;
					
						mavlink_msg_vfr_hud_decode(&message,&hud);
				//	printf("alt %f\n",hud.alt);
					current->setAltitude(hud.alt);
				
				
				break;

				case MAVLINK_MSG_ID_GPS_RAW_INT:
				
		
				mavlink_gps_raw_int_t gps;
				mavlink_msg_gps_raw_int_decode(&message, &gps);
			
				Geopoint pos(gps.lat/1E7,gps.lon/1E7,gps.alt);
				cout << "GPS" << pos.getlat() << " " << pos.getlon()  << " " << pos.getalt() << endl;
				current->update_position(pos);
				break;
				
		
			
				// GPS 
				
			}	
}


void Swarm::run() {
	
cout << "Starting SWARM" << endl;
	//while(started)
	//{
		//repulsive.execute(drones);
		Geopoint pos = drones[1]->getPosition();
			  pos.setalt(drones[1]->getBaroAltitude());
		std::list<Geopoint> resultcircle = pos.createCircle(10,4);
	

	for (std::list<Geopoint>::const_iterator iterator = resultcircle.begin(), end = resultcircle.end(); iterator != end; ++iterator) {
		Geopoint r = *iterator;

		  printf("%f %f  %f \n",r.getlat(),r.getlon(),r.getalt());
	
	  
	  mavlink->flyhere(*drones[1],r);
	  
	  sleep(2);
	}
	
	//}
}

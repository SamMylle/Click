#ifndef CLICK_SERVERINTERFACE_HH
#define CLICK_SERVERINTERFACE_HH

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>

CLICK_DECLS

class RouterRecord;

class ServerInterface: public Element{
	/// I made a mistake, it should be a RouterInterface, not ServerInterface
	/// Set IGMP queries on output 0, set IGMP packets on output 1 and other IP stuff on output 2
public:
	ServerInterface();
	~ServerInterface();
	
	const char *class_name() const	{ return "ServerInterface"; }
	const char *port_count() const	{ return "1/3"; }
	const char *processing() const	{ return PUSH; }
	int configure(Vector<String>&, ErrorHandler*);
	
	///void run_timer(Timer *);

	///static int configureSettings(const String &conf, Element *e, void * thunk, ErrorHandler * errh);
	// static String getSettings(Element *e, void * thunk);
	// void add_handlers();

	void push(int, Packet*);

private:

	void interpretGroupReport(Packet* p);
	void sendSpecificQuery(IPAddress multicastAddress);
	void updateInterface(Vector<IPAddress>& toListen, Vector<IPAddress>& toQuery);

	uint8_t f_maximumMaxRespCode;
	bool f_SFlag;
	uint8_t f_QRV;
	uint8_t f_QQIC;
	unsigned int f_queryInterval;
		/// Send general query every x seconds
	unsigned int f_queryResponseInterval;
		/// divide by 10 = seconds
		/// MaxResp time for general queries
	double f_groupMembershipInterval;
		/// Timer timeout => group has no members
	/// TODO other querier present interval and others
	unsigned int f_lastMemberQueryCount;
		/// amt of group-specific queries sent before the router assumes there are no members
	unsigned int f_lastMemberQueryTime;
		/// see rfc
	unsigned int f_lastMemberQueryInterval;
		/// maxresp time used to calc the maxrespcode in group-specific queries


	Vector<RouterRecord> f_state;
};

class RouterRecord{
public:
	RouterRecord(IPAddress ip, uint8_t filterMode, double timeOut, Element* parentInterface);
		/// Note that everything is assumed to be correct
	~RouterRecord();

	void runTimer();
	void refreshInterest();

	IPAddress f_ip;
	Timer* f_groupTimer;
	uint8_t f_filterMode;
	unsigned int f_timeOut;
	bool f_timerExpired;
	Element* f_parentInterface;

};

void run_timer(Timer* timer, void* routerRecord);


CLICK_ENDDECLS

#endif
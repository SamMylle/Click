#ifndef CLICK_GROUPRECORDGENERATOR_HH
#define CLICK_GROUPRECORDGENERATOR_HH

#include <click/config.h>
#include <click/confparse.hh>
#include <click/error.hh>
#include <clicknet/ip.h>
#include <clicknet/ether.h>
#include <click/timer.hh>


#ifndef MODE_IS_INCLUDE
#define MODE_IS_INCLUDE 1
#endif

#ifndef MODE_IS_EXCLUDE
#define MODE_IS_EXCLUDE 2
#endif

#ifndef CHANGE_TO_INCLUDE
#define CHANGE_TO_INCLUDE 3
#endif

#ifndef CHANGE_TO_EXCLUDE
#define CHANGE_TO_EXCLUDE 4
#endif

#ifndef SUPPRESS_OUTPUT
#define SUPPRESS_OUTPUT false
#endif


CLICK_DECLS


class GroupRecordGenerator{
public:
	GroupRecordGenerator();
	~GroupRecordGenerator();

	bool initNewRecord(uint8_t recordType, uint8_t auxDataLen, uint16_t nrOfSources, struct in_addr multicastAddress);
		/// Since we work with a vector, the number of sources isn't actually needed
		/// BUT the size of the vector can be greater than the size of an uint16_t
		/// I'm sure you can easily check for that, but this way had the most advantages
	bool addSourceAddress(struct in_addr unicastAddress);

	struct GroupRecord* getCurrentRecord() const;
		/// Note: this returns a copy of the current made record
		/// This grouprecord is now responsible for deleting itself properly
		/// returns a nullpointer if the current record would be invalid

	bool setRecordType(int8_t recordType);
private:
	void flushPreviousRecord();

	uint8_t f_recordType;
	uint8_t f_auxDataLen;
	uint16_t f_nrOfSources;
	struct in_addr f_multicastAddress;
	Vector<struct in_addr> f_sourceList;

	uint16_t f_addressesGiven;
	bool f_makingRecord;
};

struct GroupRecord{
	uint8_t recordType;
	uint8_t auxDataLen;
	uint16_t nrOfSources;
	struct in_addr multicastAddress;
	///struct in_addr* sourceAddresses;
};

struct GroupReport{
	uint8_t reportType;
	uint8_t reserved1;
	uint16_t checksum;
	uint16_t reserved2;
	uint16_t nrOfRecords;
	///struct GroupRecord* records;
};


class GroupReportGenerator: public Element{
public:
	GroupReportGenerator();
	~GroupReportGenerator();
	
	const char *class_name() const	{ return "GroupReportGenerator"; }
	const char *port_count() const	{ return "0/1"; }
	const char *processing() const	{ return PUSH; }
	int configure(Vector<String>&, ErrorHandler*);
	
	void run_timer(Timer *);

private:		
	Packet* make_packet();

	Vector<struct GroupRecord> f_groupRecordList;
	IPAddress f_src;
	IPAddress f_dst;
};

CLICK_ENDDECLS

#endif
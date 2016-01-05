
#ifndef TPPARSER_H_
#define TPPARSER_H_

#include "HashSet.h"
#include "TrackerData.h"
#include "FirstPartyHost.h"


class CTPParser {
public:
    CTPParser();
    ~CTPParser();
    
    void addTracker(const char *inputHost);
    // thirdPartyHosts comma separated list
    void addFirstPartyHosts(const char *inputHost, const char *thirdPartyHosts);
    bool matchesTracker(const char *inputHost);
    // Returns third party hosts as comma separated list
    // The returned buffer should be deleted.
    char* findFirstPartyHosts(const char *inputHost);
    
    // Serializes the parsed data into a single buffer.
    // The returned buffer should be deleted.
    char* serialize(unsigned int* totalSize);
    // Deserializes the buffer, a size is not needed since a serialized
    // buffer is self described
    void deserialize(char *buffer);
    
private:
    bool trackerExist(const char *inputHost);
    char* firstPartyHosts(const char *inputHost);
    
    HashSet<ST_TRACKER_DATA> mTrackers;
    HashSet<ST_FIRST_PARTY_HOST> mFirstPartyHosts;
};

#endif  //TPPARSER_H_
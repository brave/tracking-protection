#include "TPParser.h"

CTPParser::CTPParser():
    mTrackers(2196),
    mFirstPartyHosts(1016) {
}

CTPParser::~CTPParser() {
}

void CTPParser::addTracker(const char *inputHost) {
    if (nullptr == inputHost) {
        return;
    }
    ST_TRACKER_DATA trackerData;
    trackerData.sHost = new char[strlen(inputHost) + 1];
    strcpy(trackerData.sHost, inputHost);
    
    if (nullptr == trackerData.sHost) {
        return;
    }
    
    mTrackers.add(trackerData);
}

void CTPParser::addFirstPartyHosts(const char *inputHost, const char *thirdPartyHosts) {
    if (nullptr == inputHost || nullptr == thirdPartyHosts) {
        return;
    }
    
    ST_FIRST_PARTY_HOST firstPartyHost;
    firstPartyHost.sFirstPartyHost = new char[strlen(inputHost) + 1];
    firstPartyHost.sThirdPartyHosts = new char[strlen(thirdPartyHosts) + 1];
    
    if (nullptr == firstPartyHost.sFirstPartyHost || nullptr == firstPartyHost.sThirdPartyHosts) {
        return;
    }
    strcpy(firstPartyHost.sFirstPartyHost, inputHost);
    strcpy(firstPartyHost.sThirdPartyHosts, thirdPartyHosts);
    
    mFirstPartyHosts.add(firstPartyHost);
}

bool CTPParser::trackerExist(const char *inputHost) {
    ST_TRACKER_DATA trackerData;
    trackerData.sHost = new char[strlen(inputHost) + 1];
    if (nullptr == trackerData.sHost) {
        return false;
    }
    strcpy(trackerData.sHost, inputHost);
    
    return mTrackers.exists(trackerData);
}

bool CTPParser::matchesTracker(const char *inputHost) {
    if (nullptr == inputHost) {
        return false;
    }
    
    bool exist = trackerExist(inputHost);
    if (!exist) {
        unsigned int len = strlen(inputHost);
        unsigned positionToStart = 0;
        do {
            unsigned int firstDotPos = positionToStart;
            while (firstDotPos < len) {
                if ('.' == inputHost[firstDotPos]) {
                    break;
                }
                firstDotPos++;
            }
            if (firstDotPos >= len || '.' != inputHost[firstDotPos]) {
                break;
            }
            unsigned int secondDotPos = firstDotPos + 1;
            while (secondDotPos < len) {
                if ('.' == inputHost[secondDotPos]) {
                    break;
                }
                secondDotPos++;
            }
            if (secondDotPos >= len || '.' != inputHost[secondDotPos]) {
                break;
            }
            exist = trackerExist(inputHost + firstDotPos + 1);
            if (exist) {
                break;
            }
            positionToStart = firstDotPos + 1;
        } while (true);
    }
    
    return exist;
}

char* CTPParser::firstPartyHosts(const char *inputHost)
{
    ST_FIRST_PARTY_HOST firstPartyHost;
    firstPartyHost.sFirstPartyHost = new char[strlen(inputHost) + 1];
    firstPartyHost.sThirdPartyHosts = new char[1];
    
    if (nullptr == firstPartyHost.sFirstPartyHost || nullptr == firstPartyHost.sThirdPartyHosts) {
        return nullptr;
    }
    strcpy(firstPartyHost.sFirstPartyHost, inputHost);
    strcpy(firstPartyHost.sThirdPartyHosts, "");
    
    
    ST_FIRST_PARTY_HOST* foundFirstPartyHost = mFirstPartyHosts.find(firstPartyHost);
    
    if (nullptr == foundFirstPartyHost) {
        return nullptr;
    }
    
    return foundFirstPartyHost->sThirdPartyHosts;
}

char* CTPParser::findFirstPartyHosts(const char *inputHost) {
    if (nullptr == inputHost) {
        return nullptr;
    }
    
    char* result = nullptr;
    char* hosts = firstPartyHosts(inputHost);
    if (hosts) {
        result = new char[strlen(hosts) + 1];
        if (!result) {
            return nullptr;
        }
        strcpy(result, hosts);
    }
    
    unsigned int len = strlen(inputHost);
    unsigned positionToStart = 0;
    do {
        unsigned int firstDotPos = positionToStart;
        while (firstDotPos < len) {
            if ('.' == inputHost[firstDotPos]) {
                break;
            }
            firstDotPos++;
        }
        if (firstDotPos >= len || '.' != inputHost[firstDotPos]) {
            break;
        }
        unsigned int secondDotPos = firstDotPos + 1;
        while (secondDotPos < len) {
            if ('.' == inputHost[secondDotPos]) {
                break;
            }
            secondDotPos++;
        }
        if (secondDotPos >= len || '.' != inputHost[secondDotPos]) {
            break;
        }
        hosts = firstPartyHosts(inputHost + firstDotPos + 1);
        if (hosts) {
            unsigned int tempLen = strlen(hosts) + 1;
            if (result) {
                tempLen += strlen(result);
            }
            char* tempResult = new char[tempLen];
            if (!tempResult) {
                return nullptr;
            }
            tempResult[0] = '\0';
            if (result) {
                strcat(tempResult, result);
                strcat(tempResult, ",");
                delete []result;
            }
            strcat(tempResult, hosts);
            result = tempResult;
        }
        positionToStart = firstDotPos + 1;
    } while (true);
    
    return result;
}

// Returns a newly allocated buffer, caller must manually delete[] the buffer
char* CTPParser::serialize(unsigned int* totalSize) {
    *totalSize = 0;
    
    uint32_t trackersSize = 0;
    uint32_t firstPartiesSize = 0;
    char* trackers = mTrackers.serialize(&trackersSize);
    char* firstParties = mFirstPartyHosts.serialize(&firstPartiesSize);
    
    if (!trackers || !firstParties) {
        if (trackers) {
            delete []trackers;
        }
        if (firstParties) {
            delete []firstParties;
        }
        
        return nullptr;
    }
    
    *totalSize = sizeof(trackersSize) + trackersSize + sizeof(firstPartiesSize) + firstPartiesSize;
    
    unsigned int pos = 0;
    char* result = new char[*totalSize];
    if (!result) {
        delete []trackers;
        delete []firstParties;
        
        return nullptr;
    }
    memset(result, 0, *totalSize);
    
    char sz[32];
    uint32_t dataLenSize = 1 + snprintf(sz, sizeof(sz), "%x", trackersSize);
    memcpy(result + pos, sz, dataLenSize);
    pos += dataLenSize;
    
    memcpy(result + pos, trackers, trackersSize);
    pos += trackersSize;
    
    dataLenSize = 1 + snprintf(sz, sizeof(sz), "%x", firstPartiesSize);
    memcpy(result + pos, sz, dataLenSize);
    pos += dataLenSize;
    memcpy(result + pos, firstParties, firstPartiesSize);
    
    delete []trackers;
    delete []firstParties;
    
    return result;
}

void CTPParser::deserialize(char *buffer) {
    if (!buffer) {
        return;
    }
    
    uint32_t trackersSize = 0;
    unsigned int pos = 0;
    sscanf(buffer, "%x", &trackersSize);
    pos += strlen(buffer) + 1;
    
    mTrackers.deserialize(buffer + pos, trackersSize);
    pos += trackersSize;
    
    uint32_t firstPartiesSize = 0;
    sscanf(buffer + pos, "%x", &firstPartiesSize);
    pos += strlen(buffer + pos) + 1;
    mFirstPartyHosts.deserialize(buffer + pos, firstPartiesSize);
}

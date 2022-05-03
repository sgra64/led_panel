#include "Config.h"
#include "Logger.h"
using namespace std;

namespace LoggerNS {
#ifdef LOGGING
// log is a global Logger instance
Logger log(true);       // with logging enabled
#else
Logger log(false);      // with logging disabled
#endif // LOGGING
}

/*
 * Logger class method implementations
 */
void LoggerNS::Logger::log(string msg) {
    if(active) {
        printf("%s\n", msg.c_str());
        fflush(stdout);
    }
}

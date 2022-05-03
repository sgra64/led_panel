#ifndef LOGGER_H
#define LOGGER_H
#include <string>

/**
 * @brief The Logger class is a simple logger class.
 *
 * @author sgra64
 *
 */
namespace LoggerNS {

class Logger {              // Logger "declaration" (no memory allocated)
    bool active = false;
  public:
    Logger(bool active) : active(active) { }    // public constructor
    ~Logger() { }               // public destructor

    void log(std::string msg);  // member log method
};

extern Logger log;              // external Logger instance

} // namespace LoggerNS
#endif // LOGGER_H

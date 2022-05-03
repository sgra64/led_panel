#include "Config.h"
#if defined(CONTROLLER) || \
    defined(CONTROLLER_FULL)    // subclasses needs base class implementations
//
#include <string>
#include "Controller.h"
#include "Logger.h"
using namespace std;
using namespace ControllerNS;
using namespace LoggerNS;


/**
 * @brief implementations of Controller methods including methods
 * inherited from the Reactor interface.
 *
 * @author sgra64
 *
 */

//namespace ControllerNS {

int i = 0;      // global(!) int variables to control state of
int j = 0;      // left and right LEDBars: 0-off, 1-on

//}


void Controller::start() {
    log.log("Controller started, Controller::start() called.");
    controlledUnit.clockInterval(-1);     // disable clock events
}

void Controller::stop() {
    log.log("Controller stopped, Controller::stop() called.");
    controlledUnit.clockInterval(-1);     // disable clock events
}

void Controller::started() {
    log.log("started_Action();");
}

void Controller::stopped() {
    log.log("stopped_Action();");
}

void Controller::exited() {
    log.log("exit_Action();");
    exit(0 );
}

void Controller::left_Indicator() {
    i = i==0? 1 : 0;    //controlledUnit.toggleLeftLED();
    log.log("left_Action( " + std::to_string( i ) + " )");
}

void Controller::right_Indicator() {
    j = j==0? 1 : 0;    //controlledUnit.toggleRightLED();
    log.log("right_Action( " + std::to_string( j ) + " )");
}

void Controller::warn_Indicator() {
    log.log("warn_Action();");
}

void Controller::doorLock_Indicator(bool locked) {
    log.log("doorLock_Action( " + std::string(locked? "locked":"unlocked") + " );");
}

void Controller::clocktick() {
    log.log("clocktick();");
}

#endif // CONTROLLER

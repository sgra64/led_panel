#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "LEDPanel.h"
using namespace LEDPanelNS;

/**
 * @brief The Controller class defines a controller for the LED-Panel
 * application. It connects to an underlying Controlled Unit (CU)
 * controlling it through the Interface::Actor interface.
 *
 * Controller class implements the LEDPanel_Interface::Reactor interface
 * to react to upcalls from the Controlled Unit (CU).
 *
 * @author sgra64
 *
 */
namespace ControllerNS {

class Controller : public Reactor {

  protected:
    /*
     * Local reference to Controlled Unit (CU).
     */
    Actor& controlledUnit;

    /*
     * Reactor interface defines methods the LEDPanel can call back on
     * the Controller (Controller <- LEDPanel).
     *
     * This interface needs to be implemented by the Controller.
     */
    void started() override;            // unit signals it started
    void stopped() override;            // unit signals it stopped
    void exited() override;             // unit signals it is about to terminate/exit
    //
    void left_Indicator() override;     // left indicator pressed
    void right_Indicator() override;    // right indicator pressed
    void warn_Indicator() override;     // warn indicator pressed
    void doorLock_Indicator(bool locked) override;  // unit signals doorlock
    //
    void clocktick() override;          // periodically called for clock events

  public:
    Controller(Actor& cu) : controlledUnit(cu) {
        /*
         * Inject Controller reference into CU passed as reference to
         * this constructor to allow invocations of the Controller's
         * Reactor interface by the CU.
         */
        this->controlledUnit.inject(*this);
    }

    void start();   // start Controller
    void stop();    // stop Controller
};

} // namespace ControllerNS
#endif // CONTROLLER_H

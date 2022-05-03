#ifndef LEDPANEL_H
#define LEDPANEL_H

/**
 * @brief The abstract class defines interfaces between a Controller
 * and underlying controlled units.
 *
 * Interface decouple dependencies between Controllers and controlled
 * units allowing changes in implementations without affecting each
 * other.
 *
 * The interface is bi-directional and therefore has two parts:
 *  - the Actor interface defines methods the Controller calls on
 *    the controlled unit (Controller -> LEDPanel), e.g. to turn LED on or off
 *
 *  - the Reactor interface defines methods the controlled unit can invoke
 *    on the Controller (Controller <- LEDPanel), e.g. when GUI actions or
 *    time signals need to be propagated to the Controller.
 *
 * @author sgra64
 *
 */
namespace LEDPanelNS {

/*
 * Interface for a left or right LED bar to turn on all or individual
 * LED(i) with rgb >= 0, or off with rgb: -1.
 */
class LEDBar {
  public:
    virtual int set(int rgb) = 0;           // set whole bar to rgb-color, returns rgb
    virtual void set(int i, int rgb) = 0;   // set i-th LED to rgb-color
};

class Reactor;  // forward declaration for Reactor class below

/*
 * Actor interface is used by a controller to instruct the LEDPanel.
 * (Controller -> LEDPanel).
 *
 * The interface must be implemented by the underlying LEDPanel emulator.
 */
class Actor {
  public:
    virtual void start() = 0;           // start unit
    virtual void stop() = 0;            // stop unit
    //
    virtual int toggleLeftLED() = 0;    // toggle left LEDBar, returns 0-off, 1-on
    virtual int toggleRightLED() = 0;   // toggle right LEDBar, returns 0-off, 1-on
    //
    virtual LEDBar& leftLED() = 0;      // return reference to left LEDBar (interface)
    virtual LEDBar& rightLED() = 0;     // return reference to right LEDBar (interface)
    //
    virtual int clockInterval(int msec) = 0;  // set clock interval
    //
    // inject controller reference to allow invoking methods of the Reactor interface
    virtual void inject(Reactor& controller) = 0;

    enum IndicatorSelector { ON_OFF, EXIT, LEFT, RIGHT, WARN, LOCK_UNLOCK };
    // alter elements in control panel on GUI
    virtual void control_panel(IndicatorSelector selector, int value) = 0;
};

/*
 * Reactor interface defines methods the LEDPanel can call back on
 * the Controller (Controller <- LEDPanel).
 *
 * This interface needs to be implemented by the Controller.
 */
class Reactor {
  public:
    virtual void started() = 0;     // unit signals it started
    virtual void stopped() = 0;     // unit signals it stopped
    virtual void exited() = 0;      // unit signals it is about to terminate/exit
    //
    virtual void left_Indicator() = 0;      // left indicator pressed
    virtual void right_Indicator() = 0;     // right indicator pressed
    virtual void warn_Indicator() = 0;      // warn indicator pressed
    virtual void doorLock_Indicator(bool locked) = 0;   // unit signals doorlock
    //
    virtual void clocktick() = 0;   // periodically called for clock events
};

/*
 * static helper method to create color int value from Red,
 * Green and Blue values.
 */
extern int color(short r, short g, short b);

/*
 * LED colors created from RGB-values.
 */
static const int COLOR_YELLOW_1 = color(255, 236, 96);
static const int COLOR_YELLOW_2 = color(255, 255, 102);
static const int COLOR_ORANGE   = color(255, 173, 122);
static const int COLOR_OFF = -1;

} // namespace LEDPanelNS
#endif // LEDPANEL_H

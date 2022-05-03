#ifndef LEDPANEL_QTEMULATOR_H
#define LEDPANEL_QTEMULATOR_H
//
#include <QMainWindow>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>
//
#include "LEDPanel.h"
using namespace LEDPanelNS;

//QT_BEGIN_NAMESPACE
namespace Ui {
  class MainWindow;
}
//QT_END_NAMESPACE


/**
 * @brief The QtLEDBar class defines the implementation of one
 * (left or right) LEDBar. It implements the Interface::LEDBar
 * interface.
 *
 * @author sgra64
 *
 */
namespace LEDPanelNS {

class QtLEDBar final : public LEDBar {
    // only MainWindow class can construct/destruct/access
    friend class MainWindow;

    const int len;          // lenght of qtLED[] array
    QPushButton** qtLED;    // LED array [0..len-1] of QPushButton
    bool active;            // LEDBar is active

  public:
    QtLEDBar(QPushButton* qtLED[], int len);  // constructor
    ~QtLEDBar();                                // destructor to free qtLED[]

    /*
     * methods implementing the inherited Interface::LEDBar
     */
    int set(int rgb) override;
    void set(int i, int rgb) override ;
    //
    void set(bool active);
};


/**
 * @brief The MainWindow class extends the QMainWindow class Qt
 * created from the GUI-model (resources/LEDPanel_QtEmulator.ui)
 * designed with QtCreator.
 *
 * From the GUI-model, code is generated that implements the GUI:
 *  - build/ui_LEDPanel_QtEmulator.h
 *  - plus .cpp in build/{debug,profile,release}
 *
 * The MainWindow class allows to introduce extensions and access
 * GUI elements via the passed 'ui'-pointer and slot methods for
 * actions.
 *
 * @author svgr64
 *
 */
class MainWindow : public QMainWindow, public Actor {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;

    bool enableClockButton = false;
    bool enableClockControl = false;
    bool enableSoundControl = false;
    bool enableAnimationControl = false;

    QTimer *timer;

    QtLEDBar* leftLEDBar;
    QtLEDBar* rightLEDBar;

    int leftLEDBarColor = COLOR_OFF;
    int rightLEDBarColor = COLOR_OFF;

    bool active = false;
    bool doorlockToggle = true;

    /*
     * pointer to instance of Reactor API
     */
    Reactor* reactor;

    /*
     * Actor API provided by GUI, invoked by controller
     */
    void start() override { set( true ); }
    void stop() override { set( false ); }
    int toggleLeftLED() override {
        leftLEDBarColor = leftLEDBar->set(leftLEDBarColor==COLOR_OFF? COLOR_YELLOW_2 : COLOR_OFF);
        return leftLEDBarColor==COLOR_OFF? 0 : 1;
    }
    int toggleRightLED() override {
        rightLEDBarColor = rightLEDBar->set(rightLEDBarColor==COLOR_OFF? COLOR_YELLOW_2 : COLOR_OFF);
        return rightLEDBarColor==COLOR_OFF? 0 : 1;
    }
    LEDBar& leftLED() override { return *leftLEDBar; };
    LEDBar& rightLED() override { return *rightLEDBar; };
    int clockInterval(int msec) override;
    void control_panel(IndicatorSelector selector, int value) override;
    void inject(Reactor& reactor) override { this->reactor = &reactor; }

    /*
     * private methods
     */
    void set(bool active);
    void controlPanel(bool active);
    void tick();

    /*
     * Qt slot methods, bindings to Qt/GUI actions
     */
  private slots:
    void on_button_ONOFF_pressed();
    void on_button_EXIT_pressed();
    void on_button_CLOCK_pressed();
    void on_button_LEFT_pressed();
    void on_button_RIGHT_pressed();
    void on_button_WARN_pressed();
    void on_button_DOORLOCK_pressed();

    void sliderValueUpdated( int i );
};

} // namespace LEDPanel
#endif // LEDPANEL_QTEMULATOR_H

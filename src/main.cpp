#include "Config.h"
#include "Controller.h"

#ifdef CONTROLLER_FULL
#include "Controller_Full.h"
#endif

#include <QApplication>
#include "LEDPanel_QtEmulator.h"
using namespace std;
using namespace ControllerNS;
using namespace LEDPanelNS;

/**
 * @brief Main function to start program.
 *
 * @param argc C-style argument count
 * @param argv C-style argument vector passed to program from command line
 * @return exit code, 0: ok, -1 or other value: failure
 *
 * @author sgra64
 *
 */
int main(int argc, char *argv[]) {

    /*
     * Create and launch Qt components (run-time system, GUI).
     */
    QApplication qtApp(argc, argv); // create qt application instance
    MainWindow mainWindow;          // create MainWindow:: (GUI) instance
    mainWindow.show();

    /*
     * Obtains access to instance of Actor interface.
     * Use MainWindow:: instance as emulated controlled unit, cast to
     * Interface::Actor since MainWindow:: implements this interface
     */
    Actor* actor = static_cast<MainWindow*>(&mainWindow);

    /*
     * Create Controller object and pass Actor reference.
     */
#if defined(CONTROLLER)
    Controller controller(*actor);         // Controller implementations
//
#elif defined(CONTROLLER_FULL)
    Controller_Full controller(*actor);    // full Controller implementation
//
#else
    Controller controller(*actor);         // other Controller implementations
#endif
    controller.start();     // start controller
    //
    //printf( "Address Interface::supplier is %p\n", (void *)actor );
    //fflush( stdout );
    return qtApp.exec();    // launch Qt qt application instance
}

#ifndef CONFIG_H
#define CONFIG_H

// select implementation of Controller class
#define CONTROLLER            // log Actor interface actions to console
//#define CONTROLLER_D1       // Aufgabe D1
//#define CONTROLLER_D1_DONE  // Aufgabe D1 funktionsfähig

// enable/disable logging
#define LOGGING

#if ! (defined(CONTROLLER_D1) || defined(CONTROLLER_FULL))
#else
#endif

#endif // CONFIG_H

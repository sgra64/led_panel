QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#CONFIG += console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/Config.h \
    src/Controller.h \
    src/Logger.h \
    src/LEDPanel.h \
    src/LEDPanel_QtEmulator.h

SOURCES += \
    src/Controller.cpp \
    src/Controller_D1.cpp \
    src/Controller_D1_Done.cpp \
    src/LEDPanel_QtEmulator.cpp \
    src/Logger.cpp \
    src/main.cpp


RESOURCES += \
    src/resources/resources.qrc

FORMS += \
    src/resources/LEDPanel_QtEmulator.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

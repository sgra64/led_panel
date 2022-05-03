#include <iostream>

#include <Qt>
#include <QFile>
#include <QStyle>

#include "LEDPanel_QtEmulator.h"
#include "ui_LEDPanel_QtEmulator.h"


/**
 * @brief implementations for the QtLEDBar:: and MainWindow::
 * classes from LEDPanel_QtEmulator.h.
 *
 * @author sgra64
 *
 */

static const char *WINDOW_TITLE         = "LED Panel Emulator";
//
static const char *STYLESHEET_RESOURCE  = ":./stylesheet.qss";
static const char *CSS_CLASS            = "class";
static const char *CSS_CONTROL          = "control";
static const char *CSS_CONTROL_ACTIVE   = "control control-active";
static const char *CSS_LED              = "led";
static const char *CSS_LED_ACTIVE       = "led led-active";
static const char *CSS_INDICATOR_ON     = " indicator_on";
static const char *CSS_WARN_ON          = " warn_on";
static const char *CSS_DOORLOCKED       = " locked_on";

static const int CLOCKRANGE_MIN     =  1;   // range for clocktick interval
static const int CLOCKRANGE_MAX     = 30;

namespace LEDPanelNS {
  int color(short r, short g, short b) {
    int rgb = r < 0 || g < 0 || b < 0? -1 :
        (b & 0xff) << 16 | (g & 0xff) << 8 | (r & 0xff);
    return rgb;
  }
}

void styleQGroupBox(QGroupBox* grpbox, bool active, QString add_CSS_Class);

/**
 * @brief public MainWindow:: constructor
 * @param parent parent widget
 */
MainWindow::MainWindow( QWidget *parent )
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(WINDOW_TITLE);

    QPushButton* leftQtLED[] = {
        ui->LED_A01, ui->LED_A02, ui->LED_A03, ui->LED_A04,
        ui->LED_A05, ui->LED_A06, ui->LED_A07, ui->LED_A08,
        ui->LED_A09, ui->LED_A10, ui->LED_A11, ui->LED_A12,
        ui->LED_A13, ui->LED_A14, ui->LED_A15, ui->LED_A16
    };
    int leftQtLEDLenght = sizeof leftQtLED / sizeof ui->LED_A01;

    QPushButton* rightQtLED[] = {
        ui->LED_B16, ui->LED_B15, ui->LED_B14, ui->LED_B13,
        ui->LED_B12, ui->LED_B11, ui->LED_B10, ui->LED_B09,
        ui->LED_B08, ui->LED_B07, ui->LED_B06, ui->LED_B05,
        ui->LED_B04, ui->LED_B03, ui->LED_B02, ui->LED_B01
    };
    int rightQtLEDLenght = sizeof rightQtLED / sizeof ui->LED_B16;

    leftLEDBar = new QtLEDBar(leftQtLED, leftQtLEDLenght);
    rightLEDBar = new QtLEDBar(rightQtLED, rightQtLEDLenght);

    QLabel *sliderLabel = ui->label_SPEED_DISPLAY;
    sliderLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QSlider *slider = ui->slider_SPEED;
    slider->setMinimum(CLOCKRANGE_MIN);
    slider->setMaximum(CLOCKRANGE_MAX);
    int sliderInitialValue = CLOCKRANGE_MIN * 2; // =20
    slider->setValue(sliderInitialValue);
    //slider->setTickInterval(50);
    //slider->setSingleStep(50);
    slider->setTickPosition(QSlider::TicksRight); // set as scaled slider
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueUpdated(int)));

    timer = new QTimer(this );
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::tick));
    timer->start(sliderInitialValue);
    sliderValueUpdated(sliderInitialValue);

    styleQGroupBox(ui->ClockControl, false, "");
    ui->slider_SOUND->setTickPosition(QSlider::TicksRight);   // set as scaled slider

    QFile file( STYLESHEET_RESOURCE );
    file.open( QFile::ReadOnly );
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);

    leftLEDBar->set(true);    // force toggle activity state
    rightLEDBar->set(true);
    //
    controlPanel( active );
    ui->button_DOORLOCK->setText(doorlockToggle? "UNLOCK" : "LOCK");
}

/**
 * @brief public destructor MainWindow::~MainWindow
 */
MainWindow::~MainWindow() {
    delete ui;
    delete leftLEDBar;
    delete rightLEDBar;
    delete timer;
}


/*
 * MainWindow:: method implementations of Interface::Actor interface
 */

int MainWindow::clockInterval(int msec) {
    int prevValue = timer->interval();
    if( msec >= 0 ) {
        msec = msec <= CLOCKRANGE_MIN? CLOCKRANGE_MIN : msec >= CLOCKRANGE_MAX? CLOCKRANGE_MAX : msec;
        if( ! timer->isActive()) {
             timer->start();
        }
        ui->slider_SPEED->setValue(msec); // calls MainWindow::sliderValueUpdated(int i)

    } else {
        timer->stop();
        ui->slider_SPEED->setValue(CLOCKRANGE_MIN);
        ui->label_SPEED_DISPLAY->setText(QString::number(0));
    }
    return prevValue;
}

// method declaration (below)
void styleControlButton( QPushButton* button, bool active, QString add_CSS_Class );

void MainWindow::control_panel(IndicatorSelector selector, int value) {
    //
    controlPanel(active);     // reset all indicator lights
    //
    switch(selector) {
    case LEFT:
        styleControlButton(ui->button_LEFT, active, value == 0? "" : CSS_INDICATOR_ON);
        break;

    case RIGHT:
        styleControlButton(ui->button_RIGHT, active, value == 0? "" : CSS_INDICATOR_ON);
        break;

    case WARN:
        styleControlButton(ui->button_WARN, active, value == 0? "" : CSS_WARN_ON);
        break;

    case LOCK_UNLOCK:
        styleControlButton(ui->button_DOORLOCK, active, value == 0? "" : CSS_DOORLOCKED);
        break;

    default:
        break;
    }
}


/*
 * MainWindow:: private methods
 */

void MainWindow::set(bool active) {
    bool activate = ! this->active & active;
    bool deactivate = this->active & ! active;
    if( activate | deactivate ) {
        controlPanel(activate);
        this->active = activate;
        if(activate) {
            reactor->started();
        } else {    // else: deactivate
            reactor->stopped();
        }
    }
}

void styleControlButton(QPushButton* button, bool active, QString add_CSS_Class) {
    QString css_classes = (active? CSS_CONTROL_ACTIVE : CSS_CONTROL) + add_CSS_Class;
    button->setProperty(CSS_CLASS, css_classes);
    button->style()->unpolish(button);
    button->style()->polish(button);
    button->update();
    button->setEnabled(active);
}

void styleQGroupBox(QGroupBox* grpbox, bool active, QString add_CSS_Class) {
    QString css_classes = (active? CSS_CONTROL_ACTIVE : CSS_CONTROL) + add_CSS_Class;
    grpbox->setProperty(CSS_CLASS, css_classes);
    grpbox->style()->unpolish(grpbox);
    grpbox->style()->polish(grpbox);
    grpbox->update();
    grpbox->setEnabled(active);
}

void styleQComboBox(QComboBox* combox, bool active, QString add_CSS_Class) {
    QString css_classes = (active? CSS_CONTROL_ACTIVE : CSS_CONTROL) + add_CSS_Class;
    combox->setProperty(CSS_CLASS, css_classes);
    combox->style()->unpolish(combox);
    combox->style()->polish(combox);
    combox->update();
    combox->setEnabled(active);
}

void MainWindow::controlPanel(bool active) {
    QString label = active? "OFF" : "ON";
    ui->button_ONOFF->setText(label);
    styleQGroupBox(ui->IndicatorControl, active, "");
    styleControlButton(ui->button_ONOFF, true, "");

    styleControlButton(ui->button_DOORLOCK, active, doorlockToggle && active? CSS_DOORLOCKED : "");
    styleControlButton(ui->button_EXIT, active, "");
    styleControlButton(ui->button_CLOCK, enableClockButton && active, "");
    styleControlButton(ui->button_LEFT, active, "");
    styleControlButton(ui->button_WARN, active, "");
    styleControlButton(ui->button_RIGHT, active, "");

    if( ! active ) {    // reset CLOCK control visibility
        ui->button_CLOCK->setText("CLOCK");
    }
    styleQGroupBox( ui->ClockControl, enableClockControl && active, "" );
    styleQGroupBox( ui->SoundControl, enableSoundControl && active, "" );
    styleQGroupBox( ui->SoundsSelector, enableSoundControl && active, "" );

    styleQGroupBox( ui->AnimationControl, enableAnimationControl && active, "" );
    styleQComboBox( ui->comboBox_LEFT, enableAnimationControl && active, "" );
    styleQComboBox( ui->comboBox_RIGHT, enableAnimationControl && active, "" );
    styleQComboBox( ui->comboBox_WARN, enableAnimationControl && active, "" );

    leftLEDBar->set(active);
    rightLEDBar->set(active);
}

void MainWindow::tick() {
    if(active && reactor != nullptr) {
        reactor->clocktick();
    }
}


/*
 * MainWindow:: private slot methods
 */

void MainWindow::on_button_ONOFF_pressed() {
    set( ! active);
}

void MainWindow::on_button_EXIT_pressed() {
    if(reactor != nullptr) {
        reactor->exited();
    }
}

void MainWindow::on_button_CLOCK_pressed() {
    if(ui->button_CLOCK->text().compare("CLOCK") == 0 && enableClockButton) {
        ui->button_CLOCK->setText("CLOCK_OFF");
    } else {
        ui->button_CLOCK->setText("CLOCK");
    }
}

void MainWindow::on_button_LEFT_pressed() {
    if(active && reactor != nullptr) {
        reactor->left_Indicator();
    }
}

void MainWindow::on_button_RIGHT_pressed() {
    if(active && reactor != nullptr) {
        reactor->right_Indicator();
    }
}

void MainWindow::on_button_WARN_pressed() {
    if(active && reactor != nullptr) {
        reactor->warn_Indicator();
    }
}

void MainWindow::on_button_DOORLOCK_pressed() {
    if(active && reactor != nullptr) {
        doorlockToggle = ! doorlockToggle;
        reactor->doorLock_Indicator( doorlockToggle );
        ui->button_DOORLOCK->setText( doorlockToggle? "UNLOCK" : "LOCK" );
    }
}

void MainWindow::sliderValueUpdated(int i) {
    timer->setInterval(i);
    ui->label_SPEED_DISPLAY->setText( QString::number(i) );
}


/**
 * @brief public constructor QtLEDBar::QtLEDBar
 * @param qtLED array[] of GUI QPushButton's of length len
 * @param len lenght of array
 */
QtLEDBar::QtLEDBar(QPushButton* qtLED[], int len)
    : len(len), qtLED(new QPushButton*[ len ]), active(false)
{
    for(int i=0; i < len; i++) {
        this->qtLED[i] = qtLED[i];
    }
}

/**
 * @brief public destructor QtLEDBar::~QtLEDBar
 */
QtLEDBar::~QtLEDBar() {
    for(int i=0; i < len; i++) {
        this->qtLED[i]->close();
        delete this->qtLED[i];
    }
    delete[] qtLED;
}


/*
 * QtLEDBar:: private methods
 */

int QtLEDBar::set(int rgb) {
    for(int i=0; i < len; i++) {
        set(i, rgb);
    }
    return rgb;
}

void QtLEDBar::set(int i, int rgb) {
    if(i >= 0 && i < len) {
        QString css = "";
        if(rgb >= 0 && active) {
            int r = rgb & 0xff;
            int g = ( rgb >> 8 ) & 0xff;
            int b = ( rgb >> 16 ) & 0xff;
            //QString hexR = hexR.setNum( r, 16 );  // padded to 2 digits
            QString hexR = QStringLiteral( "%1" ).arg( r, 2, 16, QLatin1Char( '0' ) );
            QString hexG = QStringLiteral( "%1" ).arg( g, 2, 16, QLatin1Char( '0' ) );
            QString hexB = QStringLiteral( "%1" ).arg( b, 2, 16, QLatin1Char( '0' ) );
            css = QString( "background-color: #%1%2%3;" ).arg( hexR, hexG, hexB );
        }
        qtLED[i]->setStyleSheet(css);
    }
}

void QtLEDBar::set(bool active) {
    bool activate = ! this->active & active;
    bool deactivate = this->active & ! active;
    if(activate | deactivate) {
        for(int i=0; i < len; i++) {
            qtLED[i]->setProperty(CSS_CLASS, activate? CSS_LED_ACTIVE : CSS_LED);
            qtLED[i]->style()->unpolish(qtLED[i]);
            qtLED[i]->style()->polish(qtLED[i]);
            qtLED[i]->update();
        }
        this->active = activate;
    }
    if( ! this->active) {
        set(-1);
    }
}

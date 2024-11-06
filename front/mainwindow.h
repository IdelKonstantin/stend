#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>

#include "serialportmanager.h"
#include "cammanager.h"
#include "achtworker.h"
#include "izluchworker.h"
#include "miraworker.h"
#include "kalibrator.h"
#include "CThreadPool.h"

#include <opencv2/opencv.hpp>
#include <utility>

/*

# Install minimal prerequisites (Ubuntu 18.04 as reference)
sudo apt update && sudo apt install -y cmake g++ wget unzip

# Download and unpack sources
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.x.zip
unzip opencv.zip

# Create build directory
mkdir -p build && cd build

# Configure
cmake  ../opencv-4.x

# Build
cmake --build .

*/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setup();

private slots:

    void on_pushButton_connectPort_pressed();
    void on_pushButton_connectCamera_pressed();

    void on_checkBoxChT_clicked();
    void on_checkBoxVentilator_clicked();
    void on_pushButtonDeltaTemp_pressed();

    void on_checkBoxIzluch1_clicked();
    void on_checkBoxIzluch2_clicked();

    void on_checkBoxMiraZero_clicked();
    void on_textEditZadPolozh_cursorPositionChanged();
    void on_pushButtonKorrMinus_clicked();
    void on_pushButtonKorrPlus_clicked();

    void on_tabWidget_currentChanged(int index);
    void on_pushButtonRestart_pressed();

    void on_pushButtonIzluch1Down_pressed();
    void on_pushButtonIzluch2Down_pressed();
    void on_pushButtonIzluch1Up_pressed();
    void on_pushButtonIzluch2Up_pressed();

    void on_checkBoxResetMiraZero_clicked();

    void on_pushButtonMiraPosZad_pressed();

private:

    Ui::MainWindow *ui;

    SerialPortManager m_portManager;
    CamManager m_camManager;
    SerialPortWorker m_port;

    ACHTWorket m_achtWorker;
    IzluchWorker m_izluchWorker;
    MiraWorker m_miraWorker;
    Kalibrator m_kalibratorWorker;
    serialPortDataPreparator m_portMsgPreparator;

    threadpool::CThreadPool m_threadPool{2};

    bool m_videoThreadCanRun{true};
    bool m_UARTRxThreadCanRun{true};

private:

    bool m_userAuthorised{false};

private:

    void videoThreadWorker(QString cameraName);
    void UARTRxThreadWorker();
};
#endif // MAINWINDOW_H

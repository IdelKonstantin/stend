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
    void on_textEditDeltaTemp_cursorPositionChanged();
    void on_pushButtonDeltaTemp_pressed();

    void on_checkBoxIzluch1_clicked();
    void on_checkBoxIzluch2_clicked();
    void on_checkBoxIzluch3_clicked();
    void on_textEditZadYar_1_cursorPositionChanged();
    void on_textEditZadYar_2_cursorPositionChanged();
    void on_textEditZadYar_3_cursorPositionChanged();
    void on_pushButtonZadYark1_pressed();
    void on_pushButtonZadYark2_pressed();
    void on_pushButtonZadYark3_pressed();

    void on_checkBoxMiraZero_clicked();
    void on_textEditZadPolozh_cursorPositionChanged();
    void on_pushButtonKorrMinus_clicked();
    void on_pushButtonKorrPlus_clicked();

    void on_tabWidget_currentChanged(int index);
    void on_checkBoxDopustr_clicked();
    void on_checkBoxUstr1Zero_clicked();
    void on_checkBoxUstr2Zero_clicked();
    void on_checkBoxUstr3Zero_clicked();
    void on_textEditZadPolozh_ustr1_cursorPositionChanged();
    void on_textEditZadPolozh_ustr2_cursorPositionChanged();

    void on_pushButtonKorrPlusUstr1_clicked();
    void on_pushButtonKorrMinusUstr1_clicked();

    void on_pushButtonKorrPlusUstr2_clicked();
    void on_pushButtonKorrMinusUstr2_clicked();

    void on_pushButtonKorrPlusXUstr3_clicked();
    void on_pushButtonKorrMinusXUstr3_clicked();
    void on_pushButtonKorrPlusYUstr3_clicked();
    void on_pushButtonKorrMinusYUstr3_clicked();
    void on_pushButtonKorrPlusZUstr3_clicked();
    void on_pushButtonKorrMinusZUstr3_clicked();

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

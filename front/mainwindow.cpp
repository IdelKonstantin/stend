#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>

#include <functional>
#include <unistd.h>

#include "uartrxparser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    m_videoThreadCanRun = false;
    m_UARTRxThreadCanRun = false;
    delete ui;
}

void MainWindow::setup() {

    for(const auto& port : m_portManager.getSerialPorts()) {
        ui->comboBox_ports->addItem(port);
    }

    for(const auto& cam : m_camManager.getCams()) {
        ui->comboBox_cameras->addItem(cam);
    }

    //Current back temperature
    ui->lineEditFonTemp->setText(QString::number(m_achtWorker.getbackTemperature(), 'f', 1));

    //Current Mira position
    ui->lineEditTekPolozh->setText(QString::number(m_miraWorker.getCurrentPosition(), 'f', 1));
}

void MainWindow::on_pushButton_connectPort_pressed() {

    auto portFileName = QString{"/dev/"} + ui->comboBox_ports->currentText();

    if(m_port.connectToPort(portFileName)) {

        ui->label_portStatus->setText("Подключено");
        ui->label_portStatus->setText("OK");

        QString infoMsg = "Подключение к порту: " + portFileName + " (успешно)";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::START_UP)); 

        m_threadPool.push([this](int){UARTRxThreadWorker();});
    }
    else {

        QString errorMsg = "Не удалось подлючиться к порту: " + portFileName;
        ui->lineEditDebugPrintout->setText(errorMsg);
    }
}

void MainWindow::on_pushButton_connectCamera_pressed() {

    if(m_camManager.cameraIsConnected()) {

        QString errorMsg = "Камера уже подключена";
        ui->lineEditDebugPrintout->setText(errorMsg);
        return;
    }

    auto cameraName = ui->comboBox_cameras->currentText();
    cv::VideoCapture camera(cameraName.toStdString());

    if(!camera.isOpened()) {

        QString errorMsg = "Не удалось подключиться к камере " + cameraName;
        ui->lineEditDebugPrintout->setText(errorMsg);
        return;
    }

    camera.release();

    QString infoMsg = "Подключение к камере: " + cameraName + " (успешно)";
    ui->lineEditDebugPrintout->setText(infoMsg);
    ui->label_cameraStatus->setText("OK");

    m_threadPool.push([this, cameraName](int){videoThreadWorker(cameraName);});
}

void MainWindow::on_checkBoxChT_clicked() {

    m_achtWorker.clickTurnOnACHT();

    if(m_achtWorker.getTurnOnACHTFlag()) {
        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_ON_A4T));
            ui->lineEditDebugPrintout->setText("АЧТ включено");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
    else {
        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_OFF_A4T));
            ui->lineEditDebugPrintout->setText("АЧТ выключено");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
}

void MainWindow::on_checkBoxVentilator_clicked() {

    m_achtWorker.clickTurnOnFan();

    if(m_achtWorker.getTurnOnFanFlag()) {
        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_ON_VENT));
            ui->lineEditDebugPrintout->setText("Вентилятор включен");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
    else {
        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_OFF_VENT));
            ui->lineEditDebugPrintout->setText("Вентилятор выключен");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
}

void MainWindow::on_checkBoxIzluch1_clicked() {

    m_izluchWorker.clickTurnOnIzluch1();

    if(m_izluchWorker.getTurnOnIzluch1Flag()) {

        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_ON_ENMMITER_1));
            ui->lineEditDebugPrintout->setText("Излучатель №1 включен");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
    else {
        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_OFF_ENMMITER_1));
            ui->lineEditDebugPrintout->setText("Излучатель №1 выключен");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
}

void MainWindow::on_checkBoxIzluch2_clicked() {

    m_izluchWorker.clickTurnOnIzluch2();

    if(m_izluchWorker.getTurnOnIzluch2Flag()) {

        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_ON_ENMMITER_2));
            ui->lineEditDebugPrintout->setText("Излучатель №2 включен");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
    else {
        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_OFF_ENMMITER_2));
            ui->lineEditDebugPrintout->setText("Излучатель №2 выключен");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
}

void MainWindow::on_pushButtonDeltaTemp_pressed() {

    QString infoMsg;

    if(m_achtWorker.getTurnOnACHTFlag()) {

        auto enteredThermalDelta = m_achtWorker.getThermalDelta();
        auto enteredThermalDeltaString = QString::number(enteredThermalDelta, 'f', 1);
        auto argument = QString::number(enteredThermalDelta * 10, 'f', 0);

        if(enteredThermalDelta >= m_achtWorker.thermalDeltaMin() && enteredThermalDelta <= m_achtWorker.thermalDeltaMax() && enteredThermalDeltaString != "0.0") {

            if(m_port.isConnected()) {

                m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_TEMP_DELTA, argument));
                infoMsg = "Температурная дельта, град.= " + enteredThermalDeltaString + " отправлена в устройство.";
            }
            else {
                infoMsg = "Нет подключения к порту";
            }
        }
        else {
            infoMsg = "Температурная дельта введена не корректно.";
        }
    }
    else {
        infoMsg = "АЧТ выключено. Передача температурной дельты не возможна.";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_checkBoxMiraZero_clicked() {

    m_miraWorker.clickSaveZero();

    QString infoMsg;

    if(m_miraWorker.getSaveZerolag()) {

        if(m_port.isConnected()) {

            infoMsg = "Относительный \"нуль\" установлен";
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::SET_MIRA_ZERO));
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_textEditZadPolozh_cursorPositionChanged() {

    auto enteredContent = ui->textEditZadPolozh->toPlainText();
    bool converted{false};

    m_miraWorker.setZadPosition(enteredContent.toDouble(&converted));

    QString infoMsg;

    if (converted) {
        infoMsg = "Задано положение миры, мм. = " + QString::number(m_miraWorker.getZadPosition(), 'f', 1);
    }
    else {
        infoMsg = "Ошибка ввода положения миры. Недопустимый символ";
    }
    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrMinus_clicked() {

    m_miraWorker.minusZadPosition();

    auto miraZadPosition = m_miraWorker.getZadPosition();
    auto argument = QString::number(miraZadPosition * 10, 'f', 0);

    auto currentMiraPositionString = QString::number(miraZadPosition, 'f', 1);
    ui->textEditZadPolozh->setText(currentMiraPositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение " + currentMiraPositionString + " задано и отправлено в устройство";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_MIRA_POS, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrPlus_clicked() {

    m_miraWorker.plusZadPosition();

    auto miraZadPosition = m_miraWorker.getZadPosition();
    auto argument = QString::number(miraZadPosition * 10, 'f', 0);

    auto currentMiraPositionString = QString::number(miraZadPosition, 'f', 1);
    ui->textEditZadPolozh->setText(currentMiraPositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение " + currentMiraPositionString + " задано и отправлено в устройство";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_MIRA_POS, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_tabWidget_currentChanged(int index) {

    Q_UNUSED(index);

    QString infoMsg;

    if(ui->tabWidget->currentIndex() == 1 && !m_userAuthorised) {

        infoMsg = "Для проведения калибровки необходима аппаратная авторизация";
        ui->lineEditDebugPrintout->setText(infoMsg);
        ui->tabWidget->setCurrentIndex(0);
    }
}

void MainWindow::videoThreadWorker(QString cameraName) {

    m_camManager.setCameraConnectedFlag();
    cv::VideoCapture camera(cameraName.toStdString());
    cv::Mat frame;

    while(m_videoThreadCanRun) {

        camera >> frame;

        if(!frame.empty()) {

            auto image = QImage{frame.data, frame.cols, frame.rows, static_cast<int>(frame.step[0]), QImage::Format_Grayscale16};
            ui->label_videoStream->setPixmap(QPixmap::fromImage(image));
        }

        sleep(0L);
    }

    camera.release();
}

void MainWindow::UARTRxThreadWorker() {

    QString infoMsg;

    bool startMSGgot = false;

    uartRxParser urp;

    while(m_UARTRxThreadCanRun) {

        auto stendReplay = m_port.readMsg();

        if(!stendReplay.isEmpty()) {

            infoMsg = "Получено сообщение: " + stendReplay;
            ui->lineEditDebugPrintout->setText(infoMsg);

            if(stendReplay.indexOf("ATSTART") != -1) {
                if(!startMSGgot) {
                    //TODO: Прописать логику разбора сообщения при стартапе...
                    startMSGgot = true;
                }
            }
            else if(stendReplay.indexOf("TMP_A4T") != -1) {
                ui->lineEditFonTemp->setText(urp.getDoubleAsString(stendReplay, uartRxParser::oneDigit));
            }
        }

        sleep(0L);
    }
}


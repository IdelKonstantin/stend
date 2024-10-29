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

    for(int i = 1; i <= 10; i++) {
        ui->comboBox_tdeltas->addItem(QString::number(i));
    }
/*
    //Current back temperature
    ui->lineEditFonTemp->setText(QString::number(m_achtWorker.getbackTemperature(), 'f', 1));

    //Current Mira position
    ui->lineEditTekPolozh->setText(QString::number(m_miraWorker.getCurrentPosition(), 'f', 1));
*/
}

void MainWindow::on_pushButton_connectPort_pressed() {

    auto portFileName = QString{"/dev/"} + ui->comboBox_ports->currentText();

    if(m_port.connectToPort(portFileName)) {

        ui->label_portStatus->setText("Подключено");
        ui->label_portStatus->setText("OK");

        QString infoMsg = "Подключение к порту: " + portFileName + " (успешно)";
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

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_ON_ABB));
            ui->lineEditDebugPrintout->setText("АЧТ включено");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
    else {
        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_OFF_ABB));
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

        auto tDelta = ui->comboBox_tdeltas->currentText().toInt() * 10;
        auto tDeltaStr = QString::number(tDelta) + QString{'\n'};

        if(m_port.isConnected()) {
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::SET_THERMAL_DELTA, tDeltaStr));
            infoMsg = "Температурная дельта = " + ui->comboBox_tdeltas->currentText() + " задана";
        }
        else {
            infoMsg = "Нет подключения к порту";
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
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::MIRA_SAVE_ZERO));
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_textEditZadPolozh_cursorPositionChanged() {

    QString infoMsg;

    auto enteredContent = ui->textEditZadPolozh->toPlainText();
    bool converted{false};

    double enteredNumber = enteredContent.toDouble(&converted);

    if (!converted) {
        infoMsg = "Ошибка ввода положения миры. Недопустимый символ";
        ui->lineEditDebugPrintout->setText(infoMsg);
        return;
    }

    if (enteredNumber == 0.0) {
        infoMsg = "Ошибка ввода положения миры. Введен нуль";
        ui->lineEditDebugPrintout->setText(infoMsg);
        return;
    }

    if(enteredNumber > 0) {
        if(m_port.isConnected()) {

            infoMsg = "Задано положение миры, мм. = " + QString::number(enteredNumber, 'f', 2);
            auto steps = static_cast<uint16_t>(enteredNumber / MIN_MIRA_STEP_MM);
            auto stepsStr = QString::number(steps);
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::MIRA_MOVE_FWD, stepsStr));
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }
    else {
        if(m_port.isConnected()) {

            infoMsg = "Задано положение миры, мм. = " + QString::number(enteredNumber, 'f', 2);
            auto steps = static_cast<uint16_t>(-enteredNumber / MIN_MIRA_STEP_MM);
            auto stepsStr = QString::number(steps);
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::MIRA_MOVE_BWD, stepsStr));
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrMinus_clicked() {

    QString infoMsg;

    if(m_port.isConnected()) {
        infoMsg = "Перемещение миры на один шаг назад";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::MIRA_STEP_BWD));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrPlus_clicked() {

    QString infoMsg;

    if(m_port.isConnected()) {
        infoMsg = "Перемещение миры на один шаг вперед";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::MIRA_STEP_FWD));
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

    uartRxParser urp;

    while(m_UARTRxThreadCanRun) {

        auto stendReplay = m_port.readMsg();

        if(!stendReplay.isEmpty()) {

            infoMsg = "Получено сообщение: " + stendReplay;
            ui->lineEditDebugPrintout->setText(infoMsg);

            if(stendReplay.startsWith("CTD")) {

                auto currentDeltaStr = stendReplay.mid(3);
                auto currentTdelta = currentDeltaStr.toInt() * 0.1;
                currentDeltaStr = QString::number(currentTdelta, 'f', 1);

                if(currentDeltaStr == "0.1") {
                    currentDeltaStr = "0.0";
                }

                ui->lineEditFonTemp->setText(currentDeltaStr);
            }
            else if(stendReplay.startsWith("MMFOK") || stendReplay.startsWith("MMBOK")) {

                auto currentPos = stendReplay.mid(5).toInt() * MIN_MIRA_STEP_MM;
                auto currentPosStr = QString::number(currentPos, 'f', 2);
                ui->lineEditTekPolozh->setText(currentPosStr);
            }
            else if(stendReplay == "RSTOK") {
                ui->lineEditDebugPrintout->setText("Перезапуск стенда через 5 сек.");
            }
            else if(stendReplay == "STROK") {
                ui->lineEditDebugPrintout->setText("Стенд запущен");
                ui->lineEditFonTemp->setText("");
                ui->lineEditTekPolozh->setText("");
            }
        }
        sleep(0L);
    }
}


void MainWindow::on_pushButtonRestart_pressed()
{
    QString infoMsg;

    if(m_port.isConnected()) {

        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::STEND_RESTART));
        infoMsg = "Запрос на перезапуск стенда";
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}


void MainWindow::on_pushButtonIzluch1Down_pressed()
{
    QString infoMsg;

    if(m_izluchWorker.getTurnOnIzluch1Flag()) {

        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::EMMIT_1_INTENS_DOWN));
            infoMsg = "Уменьшение интенсивности излучателя №1";
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }
    else {
        infoMsg = "Излучатель №1 выключен";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}


void MainWindow::on_pushButtonIzluch2Down_pressed()
{
    QString infoMsg;

    if(m_izluchWorker.getTurnOnIzluch2Flag()) {

        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::EMMIT_2_INTENS_DOWN));
            infoMsg = "Уменьшение интенсивности излучателя №2";
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }
    else {
        infoMsg = "Излучатель №2 выключен";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}


void MainWindow::on_pushButtonIzluch1Up_pressed()
{
    QString infoMsg;

    if(m_izluchWorker.getTurnOnIzluch1Flag()) {

        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::EMMIT_1_INTENS_UP));
            infoMsg = "Увеличение интенсивности излучателя №1";
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }
    else {
        infoMsg = "Излучатель №1 выключен";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}


void MainWindow::on_pushButtonIzluch2Up_pressed()
{
    QString infoMsg;

    if(m_izluchWorker.getTurnOnIzluch2Flag()) {

        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::EMMIT_2_INTENS_UP));
            infoMsg = "Увеличение интенсивности излучателя №2";
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }
    else {
        infoMsg = "Излучатель №2 выключен";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_checkBoxResetMiraZero_clicked()
{
    m_miraWorker.clickResetZero();

    QString infoMsg;

    if(m_miraWorker.getResetZerolag()) {

        if(m_port.isConnected()) {

            infoMsg = "Относительный \"нуль\" сброшен";
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::MIRA_ERASE_ZERO));
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

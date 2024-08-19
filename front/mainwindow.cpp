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

    //Current immiters intensity
    ui->lineEditTekYar_1->setText(QString::number(m_izluchWorker.getIzluch1Intesity(), 'f', 1));
    ui->lineEditTekYar_2->setText(QString::number(m_izluchWorker.getIzluch2Intesity(), 'f', 1));
    ui->lineEditTekYar_3->setText(QString::number(m_izluchWorker.getIzluch3Intesity(), 'f', 1));

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

void MainWindow::on_checkBoxIzluch3_clicked() {

    m_izluchWorker.clickTurnOnIzluch3();

    if(m_izluchWorker.getTurnOnIzluch3Flag()) {

        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_ON_ENMMITER_3));
            ui->lineEditDebugPrintout->setText("Излучатель №3 включен");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
    else {
        if(m_port.isConnected()) {

            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_OFF_ENMMITER_3));
            ui->lineEditDebugPrintout->setText("Излучатель №3 выключен");
        }
        else {
            ui->lineEditDebugPrintout->setText("Нет подключения к порту");
        }
    }
}

void MainWindow::on_textEditDeltaTemp_cursorPositionChanged() {

    auto enteredContent = ui->textEditDeltaTemp->toPlainText();
    bool converted{false};

    m_achtWorker.setThermalDelta(enteredContent.toDouble(&converted));

    QString infoMsg;

    if (converted) {
        infoMsg = "Введена температурная дельта, град. = " + QString::number(m_achtWorker.getThermalDelta(), 'f', 1);
    }
    else {
        infoMsg = "Ошибка ввода температурной дельты. Недопустимый символ";
    }
    ui->lineEditDebugPrintout->setText(infoMsg);
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


void MainWindow::on_textEditZadYar_1_cursorPositionChanged() {

    auto enteredContent = ui->textEditZadYar_1->toPlainText();
    bool converted{false};

    m_izluchWorker.setIzluch1Intesity(enteredContent.toDouble(&converted));

    QString infoMsg;

    if (converted) {
        infoMsg = "Введена интенсивность излучателя №1, лк. = " + QString::number(m_izluchWorker.getIzluch1Intesity(), 'f', 1);
    }
    else {
        infoMsg = "Ошибка ввода интенсивность излучателя №1. Недопустимый символ";
    }
    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_textEditZadYar_2_cursorPositionChanged() {

    auto enteredContent = ui->textEditZadYar_2->toPlainText();
    bool converted{false};

    m_izluchWorker.setIzluch2Intesity(enteredContent.toDouble(&converted));

    QString infoMsg;

    if (converted) {
        infoMsg = "Введена интенсивность излучателя №2, лк. = " + QString::number(m_izluchWorker.getIzluch2Intesity(), 'f', 1);
    }
    else {
        infoMsg = "Ошибка ввода интенсивность излучателя №2. Недопустимый символ";
    }
    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_textEditZadYar_3_cursorPositionChanged() {

    auto enteredContent = ui->textEditZadYar_3->toPlainText();
    bool converted{false};

    m_izluchWorker.setIzluch3Intesity(enteredContent.toDouble(&converted));

    QString infoMsg;

    if (converted) {
        infoMsg = "Введена интенсивность излучателя №3, лк. = " + QString::number(m_izluchWorker.getIzluch3Intesity(), 'f', 1);
    }
    else {
        infoMsg = "Ошибка ввода интенсивность излучателя №3. Недопустимый символ";
    }
    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonZadYark1_pressed() {

    QString infoMsg;

    if(m_izluchWorker.getTurnOnIzluch1Flag()) {

        auto enteredIzluchIntensity = m_izluchWorker.getIzluch1Intesity();
        auto enteredIzluchIntensityString = QString::number(enteredIzluchIntensity, 'f', 1);
        auto argument = QString::number(enteredIzluchIntensity * 10, 'f', 0);

        auto izluchIntensityMin = m_izluchWorker.izluch1IntesityMin();
        auto izluchIntensityMax = m_izluchWorker.izluch1IntesityMax();

        if(enteredIzluchIntensity >= izluchIntensityMin && enteredIzluchIntensity <= izluchIntensityMax && enteredIzluchIntensityString != "0.0") {

            if(m_port.isConnected()) {

                infoMsg = "Интенсивность излучателя №1, лк.= " + enteredIzluchIntensityString + " отправлена в устройство.";
                m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_EMMIT_1_INTENS, argument));
            }
            else {
                infoMsg = "Нет подключения к порту";
            }
        }
        else {
            infoMsg = "Интенсивность излучателя №1 введена не корректно.";
        }
    }
    else {
        infoMsg = "Излучатель №1 выключен. Передача интенсивности не возможна";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonZadYark2_pressed() {

    QString infoMsg;

    if(m_izluchWorker.getTurnOnIzluch2Flag()) {

        auto enteredIzluchIntensity = m_izluchWorker.getIzluch2Intesity();
        auto enteredIzluchIntensityString = QString::number(enteredIzluchIntensity, 'f', 1);
        auto argument = QString::number(enteredIzluchIntensity * 10, 'f', 0);

        auto izluchIntensityMin = m_izluchWorker.izluch2IntesityMin();
        auto izluchIntensityMax = m_izluchWorker.izluch2IntesityMax();

        if(enteredIzluchIntensity >= izluchIntensityMin && enteredIzluchIntensity <= izluchIntensityMax && enteredIzluchIntensityString != "0.0") {

            if(m_port.isConnected()) {
                infoMsg = "Интенсивность излучателя №2, лк.= " + enteredIzluchIntensityString + " отправлена в устройство.";
                m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_EMMIT_2_INTENS, argument));
            }
            else {
                infoMsg = "Нет подключения к порту";
            }
        }
        else {
            infoMsg = "Интенсивность излучателя №2 введена не корректно.";
        }
    }
    else {
        infoMsg = "Излучатель №2 выключен. Передача интенсивности не возможна";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonZadYark3_pressed() {

    QString infoMsg;

    if(m_izluchWorker.getTurnOnIzluch3Flag()) {

        auto enteredIzluchIntensity = m_izluchWorker.getIzluch3Intesity();
        auto enteredIzluchIntensityString = QString::number(enteredIzluchIntensity, 'f', 1);
        auto argument = QString::number(enteredIzluchIntensity * 10, 'f', 0);

        auto izluchIntensityMin = m_izluchWorker.izluch3IntesityMin();
        auto izluchIntensityMax = m_izluchWorker.izluch3IntesityMax();

        if(enteredIzluchIntensity >= izluchIntensityMin && enteredIzluchIntensity <= izluchIntensityMax && enteredIzluchIntensityString != "0.0") {

            if(m_port.isConnected()) {
                infoMsg = "Интенсивность излучателя №3, лк.= " + enteredIzluchIntensityString + " отправлена в устройство.";
                m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_EMMIT_3_INTENS, argument));
            }
            else {
                infoMsg = "Нет подключения к порту";
            }
        }
        else {
            infoMsg = "Интенсивность излучателя №3 введена не корректно.";
        }
    }
    else {
        infoMsg = "Излучатель №3 выключен. Передача интенсивности не возможна";
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

    ui->lineEditTekuschPolozh_ustr1->setText(QString::number(m_kalibratorWorker.getCurrentPositionDev_1(), 'f', 1));
    ui->lineEditTekuschPolozh_ustr2->setText(QString::number(m_kalibratorWorker.getCurrentPositionDev_2(), 'f', 1));

    ui->lineEditTekuschPolozhX_ustr3->setText(QString::number(m_kalibratorWorker.getCurrentPositionDevX_3(), 'f', 1));
    ui->lineEditTekuschPolozhY_ustr3->setText(QString::number(m_kalibratorWorker.getCurrentPositionDevY_3(), 'f', 1));
    ui->lineEditTekuschPolozhZ_ustr3->setText(QString::number(m_kalibratorWorker.getCurrentPositionDevZ_3(), 'f', 1));
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
            else if(stendReplay.indexOf("ENT1INT") != -1) {
                ui->lineEditTekYar_1->setText(urp.getIntAsString(stendReplay));
            }
            else if(stendReplay.indexOf("ENT2INT") != -1) {
                ui->lineEditTekYar_2->setText(urp.getIntAsString(stendReplay));
            }
            else if(stendReplay.indexOf("ENT3INT") != -1) {
                ui->lineEditTekYar_3->setText(urp.getIntAsString(stendReplay));
            }
            else if(stendReplay.indexOf("MIRAPOS") != -1) {
                ui->lineEditTekPolozh->setText(urp.getIntAsString(stendReplay));
            }
            else if(stendReplay.indexOf("UST1POS") != -1) {
                if(m_userAuthorised) {
                    ui->lineEditTekuschPolozh_ustr1->setText(urp.getIntAsString(stendReplay));
                }
            }
            else if(stendReplay.indexOf("UST2POS") != -1) {
                if(m_userAuthorised) {
                    ui->lineEditTekuschPolozh_ustr2->setText(urp.getIntAsString(stendReplay));
                }
            }
            else if(stendReplay.indexOf("UST3PSX") != -1) {
                if(m_userAuthorised) {
                    ui->lineEditTekuschPolozhX_ustr3->setText(urp.getIntAsString(stendReplay));
                }
            }
            else if(stendReplay.indexOf("UST3PSY") != -1) {
                if(m_userAuthorised) {
                    ui->lineEditTekuschPolozhY_ustr3->setText(urp.getIntAsString(stendReplay));
                }
            }
            else if(stendReplay.indexOf("UST3PSZ") != -1) {
                if(m_userAuthorised) {
                    ui->lineEditTekuschPolozhZ_ustr3->setText(urp.getIntAsString(stendReplay));
                }
            }
            else if(stendReplay.indexOf("KALIBON") != -1) {
                m_userAuthorised = true;
            }
            else if(stendReplay.indexOf("KALIBOF") != -1) {
                m_userAuthorised = false;
            }
        }

        sleep(0L);
    }
}

void MainWindow::on_checkBoxDopustr_clicked() {

    m_kalibratorWorker.clickTurnOnDevs();

    QString infoMsg;
    if(m_kalibratorWorker.getTurnOnDevs()) {

        if(m_port.isConnected()) {
            infoMsg = "Устройства включены для калибровки";
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_ON_ALL_DEVICES));
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }
    else {
        if(m_port.isConnected()) {
            infoMsg = "Устройства выключены";
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::TURN_OFF_ALL_DEVICES));
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_checkBoxUstr1Zero_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.clickSaveZeroDev_1();
    QString infoMsg;

    if(m_kalibratorWorker.getSaveZeroFlagDev_1()) {

        if(m_port.isConnected()) {
            infoMsg = "Относительный \"нуль\" для устройства №1 установлен";
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::SET_ZERO_DEV_1));
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_checkBoxUstr2Zero_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.clickSaveZeroDev_2();
    QString infoMsg;

    if(m_kalibratorWorker.getSaveZeroFlagDev_2()) {

        if(m_port.isConnected()) {
            infoMsg = "Относительный \"нуль\" для устройства №2 установлен";
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::SET_ZERO_DEV_2));
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_checkBoxUstr3Zero_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.clickSaveZeroDev_3();
    QString infoMsg;

    if(m_kalibratorWorker.getSaveZeroFlagDev_3()) {

        if(m_port.isConnected()) {
            infoMsg = "Относительный \"нуль\" для устройства №3 установлен";
            m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::SET_ZERO_DEV_3));
        }
        else {
            infoMsg = "Нет подключения к порту";
        }
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_textEditZadPolozh_ustr1_cursorPositionChanged() {

    auto enteredContent = ui->textEditZadPolozh_ustr1->toPlainText();
    bool converted{false};

    m_kalibratorWorker.setZadPositionDev_1(enteredContent.toDouble(&converted));

    QString infoMsg;

    if (converted) {
        infoMsg = "Введено положение устройства №1, мм. = " + QString::number(m_kalibratorWorker.getZadPositionDev_1(), 'f', 1);
    }
    else {
        infoMsg = "Ошибка ввода заданного положения устройства №1. Недопустимый символ";
    }
    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_textEditZadPolozh_ustr2_cursorPositionChanged() {

    auto enteredContent = ui->textEditZadPolozh_ustr2->toPlainText();
    bool converted{false};

    QString infoMsg;

    m_kalibratorWorker.setZadPositionDev_2(enteredContent.toDouble(&converted));

    if (converted) {
        infoMsg = "Введено положение устройства №2, мм. = " + QString::number(m_kalibratorWorker.getZadPositionDev_2(), 'f', 1);
    }
    else {
        infoMsg = "Ошибка ввода заданного положения устройства №2. Недопустимый символ";
    }
    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrPlusUstr1_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.plusZadPositionDev_1();

    auto zadPoz = m_kalibratorWorker.getZadPositionDev_1();
    auto argument = QString::number(zadPoz * 10, 'f', 0);
    auto currentDevicePositionString = QString::number(zadPoz, 'f', 1);

    ui->textEditZadPolozh_ustr1->setText(currentDevicePositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение " + currentDevicePositionString + " задано и отправлено в устройство №1";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_POS_DEV_1, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrMinusUstr1_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.minusZadPositionDev_1();

    auto zadPoz = m_kalibratorWorker.getZadPositionDev_1();
    auto argument = QString::number(zadPoz * 10, 'f', 0);
    auto currentDevicePositionString = QString::number(zadPoz, 'f', 1);

    ui->textEditZadPolozh_ustr1->setText(currentDevicePositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение " + currentDevicePositionString + " задано и отправлено в устройство №1";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_POS_DEV_1, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrPlusUstr2_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.plusZadPositionDev_2();

    auto zadPoz = m_kalibratorWorker.getZadPositionDev_2();
    auto argument = QString::number(zadPoz * 10, 'f', 0);
    auto currentDevicePositionString = QString::number(zadPoz, 'f', 1);

    ui->textEditZadPolozh_ustr1->setText(currentDevicePositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение " + currentDevicePositionString + " задано и отправлено в устройство №2";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_POS_DEV_2, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrMinusUstr2_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.minusZadPositionDev_2();

    auto zadPoz = m_kalibratorWorker.getZadPositionDev_2();
    auto argument = QString::number(zadPoz * 10, 'f', 0);
    auto currentDevicePositionString = QString::number(zadPoz, 'f', 1);

    ui->textEditZadPolozh_ustr1->setText(currentDevicePositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение " + currentDevicePositionString + " задано и отправлено в устройство №2";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_POS_DEV_2, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrMinusXUstr3_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.minusZadPositionDevX_3();

    auto zadPoz = m_kalibratorWorker.getCurrentPositionDevX_3();
    auto argument = QString::number(zadPoz * 10, 'f', 0);
    auto currentDevicePositionString = QString::number(zadPoz, 'f', 1);

    ui->textEditZadPolozh_ustr1->setText(currentDevicePositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение по оси X = " + currentDevicePositionString + " задано и отправлено в устройство №3";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_POS_DEV_3_X, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrPlusXUstr3_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.plusZadPositionDevX_3();

    auto zadPoz = m_kalibratorWorker.getCurrentPositionDevX_3();
    auto argument = QString::number(zadPoz * 10, 'f', 0);
    auto currentDevicePositionString = QString::number(zadPoz, 'f', 1);

    ui->textEditZadPolozh_ustr1->setText(currentDevicePositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение по оси X = " + currentDevicePositionString + " задано и отправлено в устройство №3";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_POS_DEV_3_X, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrMinusYUstr3_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.minusZadPositionDevY_3();

    auto zadPoz = m_kalibratorWorker.getCurrentPositionDevY_3();
    auto argument = QString::number(zadPoz * 10, 'f', 0);
    auto currentDevicePositionString = QString::number(zadPoz, 'f', 1);

    ui->textEditZadPolozh_ustr1->setText(currentDevicePositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение по оси Y = " + currentDevicePositionString + " задано и отправлено в устройство №3";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_POS_DEV_3_Y, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrPlusYUstr3_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.plusZadPositionDevY_3();

    auto zadPoz = m_kalibratorWorker.getCurrentPositionDevY_3();
    auto argument = QString::number(zadPoz * 10, 'f', 0);
    auto currentDevicePositionString = QString::number(zadPoz, 'f', 1);

    ui->textEditZadPolozh_ustr1->setText(currentDevicePositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение по оси Y = " + currentDevicePositionString + " задано и отправлено в устройство №3";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_POS_DEV_3_Y, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrMinusZUstr3_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.minusZadPositionDevZ_3();

    auto zadPoz = m_kalibratorWorker.getCurrentPositionDevZ_3();
    auto argument = QString::number(zadPoz * 10, 'f', 0);
    auto currentDevicePositionString = QString::number(zadPoz, 'f', 1);

    ui->textEditZadPolozh_ustr1->setText(currentDevicePositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение по оси Z = " + currentDevicePositionString + " задано и отправлено в устройство №3";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_POS_DEV_3_Z, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

void MainWindow::on_pushButtonKorrPlusZUstr3_clicked() {

    if(!m_kalibratorWorker.getTurnOnDevs()) {
        ui->lineEditDebugPrintout->setText("Устройства выключены");
        return;
    }

    m_kalibratorWorker.plusZadPositionDevZ_3();

    auto zadPoz = m_kalibratorWorker.getCurrentPositionDevZ_3();
    auto argument = QString::number(zadPoz * 10, 'f', 0);
    auto currentDevicePositionString = QString::number(zadPoz, 'f', 1);

    ui->textEditZadPolozh_ustr1->setText(currentDevicePositionString);

    QString infoMsg;

    if(m_port.isConnected()) {

        infoMsg = "Перемещение по оси Z = " + currentDevicePositionString + " задано и отправлено в устройство №3";
        m_port.sendMSG(m_portMsgPreparator.prepareOutMessage(portMsg::DEF_POS_DEV_3_Z, argument));
    }
    else {
        infoMsg = "Нет подключения к порту";
    }

    ui->lineEditDebugPrintout->setText(infoMsg);
}

#include "serialportmanager.h"

#include <dirent.h>
#include <cstring>

#include <iostream>

QVector<QString> SerialPortManager::getSerialPorts() {

    QVector<QString> ports;

    struct dirent *entry;
    auto dp = opendir("/dev");

    if (dp == NULL) {

        return QVector<QString>{};
    }

    const char* ttyA = "ttyA";
    const char* ttyU = "ttyU";

    while ((entry = readdir(dp)) != NULL) {

        if (strncmp(entry->d_name, ttyA, strlen(ttyA)) == 0 ||
            strncmp(entry->d_name, ttyU, strlen(ttyU)) == 0) {

            m_selectedPort = QString{entry->d_name};

            ports.append(m_selectedPort);
        }
    }

    closedir(dp);
    return ports;
}

SerialPortWorker::SerialPortWorker() {}

SerialPortWorker::~SerialPortWorker() {

    if(m_fd != -1) {

        close(m_fd);
    }
}

bool SerialPortWorker::connectToPort(const QString& portName) {

    if(m_connected && m_previousPortName == portName) {

        return true;
    }

    m_previousPortName = portName;

    m_fd = open(portName.toStdString().c_str(), O_RDWR);

    if(m_fd == -1) {

        m_connected = false;
        return m_connected;
    }

    struct termios options;
    tcgetattr(m_fd, &options);

    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CRTSCTS;
    options.c_lflag &= ~(ECHO|ICANON|ISIG);

    options.c_cc[VMIN] = 16;
    options.c_cc[VTIME] = 10;

    tcsetattr(m_fd, TCSANOW, &options);

    m_connected = true;
    return m_connected;
}

bool SerialPortWorker::isConnected() const {

    return m_connected;
}

void SerialPortWorker::sendMSG(const QString& outMessage) const {

    write(m_fd, outMessage.toStdString().c_str(), outMessage.size());
}

const QString SerialPortWorker::readMsg() const {

    char bytesBuffer[BYTES_MESSAGE_SIZE];

    std::memset(bytesBuffer, '\0', BYTES_MESSAGE_SIZE);

    auto bytesRead = read(m_fd, &bytesBuffer, BYTES_MESSAGE_SIZE);

    std::cout << "Bytes read:" << bytesRead << std::endl;
    std::cout << "Bytes raw:" << QString{bytesBuffer}.toStdString() << std::endl;

    if(bytesRead != -1 && bytesRead != 0) {

        return QString{bytesBuffer};
    }
    return QByteArray{};
}

serialPortDataPreparator::serialPortDataPreparator() {}

QString serialPortDataPreparator::prepareOutMessage(portMsg msgType, const QString& payload) {

    switch(msgType) {

        case portMsg::SET_THERMAL_DELTA:
            return QString{"STD"} + payload + QString{'\n'};
        case portMsg::TURN_ON_ENMMITER_1:
            return QString{"LT1ON\n"};
        case portMsg::TURN_ON_ENMMITER_2:
            return QString{"LT2ON\n"};
        case portMsg::TURN_OFF_ENMMITER_1:
            return QString{"LT1OFF\n"};
        case portMsg::TURN_OFF_ENMMITER_2:
            return QString{"LT2OFF\n"};
        case portMsg::EMMIT_1_INTENS_UP:
            return QString{"LT1U\n"};
        case portMsg::EMMIT_2_INTENS_UP:
            return QString{"LT2U\n"};
        case portMsg::EMMIT_1_INTENS_DOWN:
            return QString{"LT1D\n"};
        case portMsg::EMMIT_2_INTENS_DOWN:
            return QString{"LT2D\n"};
        case portMsg::TURN_ON_ABB:
            return QString{"BBON\n"};
        case portMsg::TURN_OFF_ABB:
            return QString{"BBOFF\n"};
        case portMsg::TURN_ON_VENT:
            return QString{"VON\n"};
        case portMsg::TURN_OFF_VENT:
            return QString{"VOFF\n"};
        case portMsg::STEND_RESTART:
            return QString{"RST\n"};
        case portMsg::MIRA_STEP_FWD:
            return QString{"MFS\n"};
        case portMsg::MIRA_STEP_BWD:
            return QString{"MBS\n"};
        case portMsg::MIRA_SAVE_ZERO:
            return QString{"MSZ\n"};
        case portMsg::MIRA_ERASE_ZERO:
            return QString{"MRZ\n"};
        case portMsg::MIRA_MOVE_FWD:
            return QString{"MMF"} + payload + QString{'\n'};
        case portMsg::MIRA_MOVE_BWD:
            return QString{"MMB"} + payload + QString{'\n'};
        default:
            return QString{};
    }
}

#include "serialportmanager.h"

#include <dirent.h>
#include <cstring>

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
    options.c_lflag = 0;

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

    auto bytesRead = read(m_fd, bytesBuffer, BYTES_MESSAGE_SIZE);

    if(bytesRead != -1 && bytesRead != 0) {

        return QString{bytesBuffer};
    }
    return QByteArray{};
}

serialPortDataPreparator::serialPortDataPreparator() {}

QString serialPortDataPreparator::prepareOutMessage(portMsg msgType, const QString& payload) {

    switch(msgType) {
        case portMsg::START_UP:
            return QString{"GO\n"};
        case portMsg::TURN_ON_A4T:
            return QString{"TON_ABB\n"};
        case portMsg::TURN_OFF_A4T:
            return QString{"TOF_ABB\n"};
        case portMsg::TURN_ON_VENT:
            return QString{"TON_VNT\n"};
        case portMsg::TURN_OFF_VENT:
            return QString{"TOF_VNT\n"};
        case portMsg::DEF_TEMP_DELTA:
            return QString{"DEF_TDEL_"} + payload;
        case portMsg::TURN_ON_ENMMITER_1:
            return QString{"TON_EM1\n"};
        case portMsg::TURN_ON_ENMMITER_2:
            return QString{"TON_EM2\n"};
        case portMsg::TURN_ON_ENMMITER_3:
            return QString{"TON_EM3\n"};
        case portMsg::TURN_OFF_ENMMITER_1:
            return QString{"TOF_EM1\n"};
        case portMsg::TURN_OFF_ENMMITER_2:
            return QString{"TOF_EM2\n"};
        case portMsg::TURN_OFF_ENMMITER_3:
            return QString{"TOF_EM3\n"};
        case portMsg::DEF_EMMIT_1_INTENS:
            return QString{"DEF_E1IN_"} + payload;
        case portMsg::DEF_EMMIT_2_INTENS:
            return QString{"DEF_E2IN_"} + payload;
        case portMsg::DEF_EMMIT_3_INTENS:
            return QString{"DEF_E3IN_"} + payload;
        case portMsg::DEF_MIRA_POS:
            return QString{"DEF_MPOS_"} + payload;
        case portMsg::SET_MIRA_ZERO:
            return QString{"SETZ_MR\n"};
        case portMsg::TURN_ON_ALL_DEVICES:
            return QString{"TON_ALD\n"};
        case portMsg::TURN_OFF_ALL_DEVICES:
            return QString{"TOF_ALD\n"};
        case portMsg::DEF_POS_DEV_1:
            return QString{"DEF_D1PX_"} + payload;
        case portMsg::DEF_POS_DEV_2:
            return QString{"DEF_D2PX_"} + payload;
        case portMsg::DEF_POS_DEV_3_X:
            return QString{"DEF_D3PX_"} + payload;
        case portMsg::DEF_POS_DEV_3_Y:
            return QString{"DEF_D3PY_"} + payload;
        case portMsg::DEF_POS_DEV_3_Z:
            return QString{"DEF_D3PZ_"} + payload;
        case portMsg::SET_ZERO_DEV_1:
            return QString{"SETZ_D1\n"};
        case portMsg::SET_ZERO_DEV_2:
            return QString{"SETZ_D2\n"};
        case portMsg::SET_ZERO_DEV_3:
            return QString{"SETZ_D3\n"};
        default:
            return QString{};
    }
}

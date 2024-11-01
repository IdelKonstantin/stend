#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QString>
#include <QVector>

class SerialPortManager {

    QString m_selectedPort;

public:
    QVector<QString> getSerialPorts();
};

/////////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#define BYTES_MESSAGE_SIZE 32

class SerialPortWorker {

    int m_fd;
    bool m_connected;
    QString m_previousPortName{""};

public:
    SerialPortWorker();
    ~SerialPortWorker();

    bool connectToPort(const QString& portName);
    bool isConnected() const;

    void sendMSG(const QString& outMessage) const;
    const QString readMsg() const;
};

/////////////////////////////////////////////////////////////////////////

enum class portMsg {

    //Main
    SET_THERMAL_DELTA,
    TURN_ON_ENMMITER_1,
    TURN_ON_ENMMITER_2,
    TURN_OFF_ENMMITER_1,
    TURN_OFF_ENMMITER_2,
    EMMIT_1_INTENS_UP,
    EMMIT_2_INTENS_UP,
    EMMIT_1_INTENS_DOWN,
    EMMIT_2_INTENS_DOWN,
    TURN_ON_ABB,
    TURN_OFF_ABB,
    TURN_ON_VENT,
    TURN_OFF_VENT,
    STEND_RESTART,
    MIRA_STEP_FWD,
    MIRA_STEP_BWD,
    MIRA_SAVE_ZERO,
    MIRA_ERASE_ZERO,
    MIRA_MOVE_FWD,
    MIRA_MOVE_BWD

    /* Additional
    TURN_ON_ALL_DEVICES,
    TURN_OFF_ALL_DEVICES,
    DEF_POS_DEV_1,
    DEF_POS_DEV_2,
    DEF_POS_DEV_3_X,
    DEF_POS_DEV_3_Y,
    DEF_POS_DEV_3_Z,
    SET_ZERO_DEV_1,
    SET_ZERO_DEV_2,
    SET_ZERO_DEV_3, */
};

class serialPortDataPreparator {

private:

public:

    serialPortDataPreparator();

    QString prepareOutMessage(portMsg msgType, const QString& payload = "");
};

#endif // SERIALPORTMANAGER_H

#ifndef CAMMANAGER_H
#define CAMMANAGER_H

#include <QString>
#include <QVector>

class CamManager {

public:

    bool m_cameraConnected{false};

public:
    QVector<QString> getCams();

    bool cameraIsConnected() const;

    void setCameraConnectedFlag();
};

#endif // CAMMANAGER_H

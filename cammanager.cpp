#include "cammanager.h"

#include <dirent.h>
#include <cstring>

QVector<QString> CamManager::getCams() {

    QVector<QString> cams;
    const char* camDir = "/dev";

    struct dirent *entry;
    auto dp = opendir(camDir);

    if (dp == NULL) {
        return QVector<QString>{};
    }

    const char* camPrefix = "video";

    while ((entry = readdir(dp)) != NULL) {

        if (strncmp(entry->d_name, camPrefix, strlen(camPrefix)) == 0) {
            cams.append(QString{camDir} + QString{"/"} + QString{entry->d_name});
        }
    }

    closedir(dp);
    return cams;
}

bool CamManager::cameraIsConnected() const {

    return m_cameraConnected;
}

void CamManager::setCameraConnectedFlag() {

    m_cameraConnected = true;
}

#ifndef C_TARGET_MANAGER_H
#define C_TARGET_MANAGER_H
#include "C_radar_data.h"
#include "c_config.h"
#include <QUdpSocket>
#define TRACK_TABLE_SIZE 24
#define TARGET_TABLE_SIZE 6
#define KASU_DATA_SIZE 144
typedef struct {
    track_t* track;
    unsigned long long trackID;
}TrackPointer;


class c_target_manager
{
public:
    unsigned char kasudatagram[KASU_DATA_SIZE];

    TrackPointer    *trackTable;
    TrackPointer    *targetTable;
    QUdpSocket      *udpSocketSend;
    unsigned long long selectedTrackID =0;
    c_target_manager();
    ~c_target_manager();
    void OutputTargetData();
    void initDataGram();
};

#endif // C_TARGET_MANAGER_H

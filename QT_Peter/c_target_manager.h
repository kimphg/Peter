#ifndef C_TARGET_MANAGER_H
#define C_TARGET_MANAGER_H
#include "C_radar_data.h"
#include "c_config.h"
#include <QUdpSocket>
#define TRACK_TABLE_SIZE 100
#define TARGET_TABLE_SIZE 6
#define KASU_DATA_SIZE 144

typedef struct {
    track_t* track;
    int trackUniqID;
    bool selected;
    int flag;

}TrackPointer;


class c_target_manager
{
public:
    unsigned char kasudatagram[KASU_DATA_SIZE];
    c_target_manager();
    ~c_target_manager();
    void setCurToEnemy();
    void setCurToFriend();
    QString addCurrTrackToTargets();
    void OutputTargetToKasu();
    void initDataGram();
    void ClearTargetTable();
    TrackPointer *getTrackById(int id);
    TrackPointer *getTargetById(int id);
    TrackPointer *getTrackAt(int i);
    bool addTrack(track_t *);
    bool addTarget(track_t *);
    bool checkIDExist(int id);
    bool changeCurrTrackID(int id);
    void setSelectedTrack(int uniqID);
    TrackPointer *getTargetAt(int i);
    TrackPointer* currTrackPt;
private:
    static TrackPointer    *trackTable;
    static TrackPointer    *targetTable;
    QUdpSocket      *udpSocketSend;
};

#endif // C_TARGET_MANAGER_H

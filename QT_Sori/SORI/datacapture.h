#ifndef DATACAPTURE_H
#define DATACAPTURE_H
#include <QThread>
#include <QList>
#define HAVE_REMOTE// for pcap
#include "pcap.h"
class DataCapture : public QThread
{
public:
    //QList<pcap_if_t*> mInterfacesList;
    DataCapture();
    QString mInterface;
    void run();
    QString getInterface() ;
    void setInterface(QString interf);
    static QStringList getInterfacesList();
};

#endif // DATACAPTURE_H

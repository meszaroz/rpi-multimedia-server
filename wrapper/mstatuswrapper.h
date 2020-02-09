#ifndef MSTATUSWRAPPER_H
#define MSTATUSWRAPPER_H

#include <QString>
#include "./mabstractwrapper.h"
#include "./common/mstatus.h"

class MStatusWrapper : public MAbstractWrapper {
public:
    MStatusWrapper(Buffer *buffer);
    MStatusWrapper(const QString &act, const bool &play = false, const int &dura = 0, const int &pos = 0, const int &vol = 50);
    MStatusWrapper(const StatusContainer &cont);
    MStatusWrapper(const MStatusWrapper &other);
    MStatusWrapper& operator = (const MStatusWrapper &other);
    ~MStatusWrapper();

    bool isValid() const;
    const StatusContainer* container() const;
    const Buffer* toBuffer() const;
private:
    StatusContainer *mContainer;
};

#endif // MSTATUSWRAPPER_H

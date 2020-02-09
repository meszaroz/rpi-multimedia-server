#ifndef MABSTRACTWRAPPER_H
#define MABSTRACTWRAPPER_H

#include <QByteArray>
#include "./common/mcommon.h"

class MAbstractWrapper {
public:
    MAbstractWrapper();
    virtual ~MAbstractWrapper();
    virtual bool isValid() const;
    virtual const Buffer* toBuffer() const = 0;
    virtual const QByteArray* toStreamByteArray() const;
protected:
    mutable Buffer *mBuffer;
    mutable QByteArray *mData;
};

#endif // MABSTRACTWRAPPER_H

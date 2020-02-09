#include "mabstractwrapper.h"

MAbstractWrapper::MAbstractWrapper() :
    mBuffer(0), mData(0)
{}

MAbstractWrapper::~MAbstractWrapper() {
    clearBuffer(mBuffer);
    delete mData;
}

bool MAbstractWrapper::isValid() const {
    return true;
}

const QByteArray *MAbstractWrapper::toStreamByteArray() const {
    if (!mData) {
        const Buffer *buffer = toBuffer();
        if (buffer) {
            TSize size;
            TPointer *data = bufferToStreamData(buffer,&size);
            if (data) {
                mData = new QByteArray((const char*)data,size);
                free(data);
            }
        }
    }
    return mData;
}


#include "./wrapper/mmessagewrapper.h"

MMessageWrapper::MMessageWrapper(Buffer *buffer) :
    mContainer(readMessageContainerFromBuffer(buffer))
{}

MMessageWrapper::MMessageWrapper(const QString &message) :
    mContainer(createMessageContainer())
{
    if (mContainer) {
        copyString(&mContainer->message,message.toStdString().c_str());
    }
}

MMessageWrapper::MMessageWrapper(const MessageContainer &cont) :
    mContainer(copyMessageContainer(&cont))
{}

MMessageWrapper::MMessageWrapper(const MMessageWrapper &other) :
    mContainer(other.isValid() ? copyMessageContainer(other.container()) : 0)
{}

MMessageWrapper& MMessageWrapper::operator=(const MMessageWrapper &other)
{
    if (&other != this && other.isValid()) {
        clearMessageContainer(mContainer);
        mContainer = copyMessageContainer(other.container());
    }
    return *this;
}

MMessageWrapper::~MMessageWrapper() {
    clearMessageContainer(mContainer);
}

bool MMessageWrapper::isValid() const {
    return mContainer;
}

const MessageContainer* MMessageWrapper::container() const {
    return mContainer;
}

const Buffer* MMessageWrapper::toBuffer() const {
    if (!mBuffer && isValid()) {
        mBuffer = writeMessageContainerToBuffer(mContainer);
    }
    return mBuffer;
}


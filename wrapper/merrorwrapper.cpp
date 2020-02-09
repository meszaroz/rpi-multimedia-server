#include "./wrapper/merrorwrapper.h"

MErrorWrapper::MErrorWrapper(Buffer *buffer) :
    mContainer(readErrorContainerFromBuffer(buffer))
{}

MErrorWrapper::MErrorWrapper(const QString &message) :
    mContainer(createErrorContainer())
{
    if (mContainer) {
        copyString(&mContainer->message,message.toStdString().c_str());
    }
}

MErrorWrapper::MErrorWrapper(const ErrorContainer &cont) :
    mContainer(copyErrorContainer(&cont))
{}

MErrorWrapper::MErrorWrapper(const MErrorWrapper &other) :
    mContainer(other.isValid() ? copyErrorContainer(other.container()) : 0)
{}

MErrorWrapper& MErrorWrapper::operator=(const MErrorWrapper &other)
{
    if (&other != this && other.isValid()) {
        clearErrorContainer(mContainer);
        mContainer = copyErrorContainer(other.container());
    }
    return *this;
}

MErrorWrapper::~MErrorWrapper() {
    clearErrorContainer(mContainer);
}

bool MErrorWrapper::isValid() const {
    return mContainer;
}

const ErrorContainer* MErrorWrapper::container() const {
    return mContainer;
}

const Buffer* MErrorWrapper::toBuffer() const {
    if (!mBuffer && isValid()) {
        mBuffer = writeErrorContainerToBuffer(mContainer);
    }
    return mBuffer;
}


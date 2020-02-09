#include "./wrapper/mcommandwrapper.h"

MCommandWrapper::MCommandWrapper(Buffer *buffer) :
    mContainer(readCommandContainerFromBuffer(buffer))
{
}

MCommandWrapper::MCommandWrapper(const CommandContainer &cont) :
    mContainer(copyCommandContainer(&cont))
{}

MCommandWrapper::~MCommandWrapper() {
    clearCommandContainer(mContainer);
}

bool MCommandWrapper::isValid() const {
    return mContainer && mContainer->cmd < Undefined;
}

const CommandContainer* MCommandWrapper::container() const {
    return mContainer;
}

const Buffer* MCommandWrapper::toBuffer() const {
    if (!mBuffer && isValid()) {
        mBuffer = writeCommandContainerToBuffer(mContainer);
    }
    return mBuffer;
}


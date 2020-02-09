#include "./wrapper/mcheckwrapper.h"

const Buffer *MCheckWrapper::toBuffer() const {
    if (!mBuffer && isValid()) {
        mBuffer = createBuffer(Check,0);
    }
    return mBuffer;
}

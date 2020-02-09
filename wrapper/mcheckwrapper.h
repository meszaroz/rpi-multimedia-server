#ifndef MCHECKWRAPPER_H
#define MCHECKWRAPPER_H

#include "./mabstractwrapper.h"

class MCheckWrapper : public MAbstractWrapper {
public:
    const Buffer* toBuffer() const;
};

#endif // MCHECKWRAPPER_H

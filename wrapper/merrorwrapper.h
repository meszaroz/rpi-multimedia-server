#ifndef MERRORWRAPPER_H
#define MERRORWRAPPER_H

#include <QString>
#include "./mabstractwrapper.h"
#include "./common/merror.h"

class MErrorWrapper : public MAbstractWrapper {
public:
    MErrorWrapper(Buffer *buffer);
    MErrorWrapper(const QString &message);
    MErrorWrapper(const ErrorContainer &cont);
    MErrorWrapper(const MErrorWrapper &other);
    MErrorWrapper& operator = (const MErrorWrapper &other);
    ~MErrorWrapper();

    bool isValid() const;
    const ErrorContainer* container() const;
    const Buffer* toBuffer() const;
private:
    ErrorContainer *mContainer;
};

#endif // MErrorWRAPPER_H

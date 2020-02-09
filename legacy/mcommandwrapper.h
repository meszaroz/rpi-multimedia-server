#ifndef MCOMMANDWRAPPER_H
#define MCOMMANDWRAPPER_H

#include "./mabstractwrapper.h"
#include "./common/mcommand.h"

class MCommandWrapper : public MAbstractWrapper {
public:
    MCommandWrapper(Buffer *buffer);
    MCommandWrapper(const CommandContainer &cont);
    ~MCommandWrapper();
    bool isValid() const;
    const CommandContainer* container() const;
    const Buffer* toBuffer() const;
private:
    CommandContainer *mContainer;
};

#endif // MCOMMANDWRAPPER_H

#ifndef MMESSAGEWRAPPER_H
#define MMESSAGEWRAPPER_H

#include <QString>
#include "./mabstractwrapper.h"
#include "./common/mmessage.h"

class MMessageWrapper : public MAbstractWrapper {
public:
    MMessageWrapper(Buffer *buffer);
    MMessageWrapper(const QString &message);
    MMessageWrapper(const MessageContainer &cont);
    MMessageWrapper(const MMessageWrapper &other);
    MMessageWrapper& operator = (const MMessageWrapper &other);
    ~MMessageWrapper();

    bool isValid() const;
    const MessageContainer* container() const;
    const Buffer* toBuffer() const;
private:
    MessageContainer *mContainer;
};

#endif // MMESSAGEWRAPPER_H

#ifndef MLISTWRAPPER
#define MLISTWRAPPER

#include <QStringList>
#include "./mabstractwrapper.h"
#include "./common/mlist.h"

class MListWrapper : public MAbstractWrapper {
public:
    MListWrapper(Buffer *buffer);
    MListWrapper(const QStringList &list);
    MListWrapper(const ListContainer &cont);
    MListWrapper(const MListWrapper &other);
    MListWrapper& operator = (const MListWrapper &other);
    ~MListWrapper();

    bool isValid() const;
    const ListContainer* container() const;
    const Buffer* toBuffer() const;
private:
    ListContainer *mContainer;
};

#endif // MLISTWRAPPER

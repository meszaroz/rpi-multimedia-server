//
//  MList.c
//  RPIMultimediaController
//
//  Created by Zoltan Meszaros on 1/24/15.
//  Copyright (c) 2015 Zoltan Meszaros. All rights reserved.
//

#include "./wrapper/mlistwrapper.h"

MListWrapper::MListWrapper(Buffer *buffer) :
    mContainer(readListContainerFromBuffer(buffer))
{}

MListWrapper::MListWrapper(const QStringList &list) :
    mContainer(createListContainer(list.count()))
{
    if (mContainer) {
        for (int i = 0; i < list.count(); ++i) {
            copyString(&mContainer->list[i],list.at(i).toStdString().c_str());
        }
    }
}

MListWrapper::MListWrapper(const ListContainer &cont) :
    mContainer(copyListContainer(&cont))
{}

MListWrapper::MListWrapper(const MListWrapper &other) :
    mContainer(other.isValid() ? copyListContainer(other.container()) : 0)
{}

MListWrapper& MListWrapper::operator=(const MListWrapper &other)
{
    if (&other != this && other.isValid()) {
        clearListContainer(mContainer);
        mContainer = copyListContainer(other.container());
    }
    return *this;
}


MListWrapper::~MListWrapper() {
    clearListContainer(mContainer);
}

bool MListWrapper::isValid() const {
    return mContainer && mContainer->list && mContainer->count > 0;
}

const ListContainer* MListWrapper::container() const {
    return mContainer;
}

const Buffer* MListWrapper::toBuffer() const {
    if (!mBuffer && isValid()) {
        mBuffer = writeListContainerToBuffer(mContainer);
    }
    return mBuffer;
}

//
//  MImage.c
//  RPIMultimediaController
//
//  Created by Zoltan Meszaros on 1/18/15.
//  Copyright (c) 2015 Zoltan Meszaros. All rights reserved.
//

#include "./wrapper/mimagewrapper.h"
#include "./resource.h"

MImageWrapper::MImageWrapper(Buffer *buffer) :
    mContainer(readImageContainerFromBuffer(buffer))
{
    loadImage();
}

MImageWrapper::MImageWrapper(const ImageContainer &cont) :
    mContainer(copyImageContainer(&cont))
{
    loadImage();
}

MImageWrapper::MImageWrapper(const MImageWrapper &other) :
    mContainer(other.isValid() ? copyImageContainer(other.container()) : 0)
{
    loadImage();
}

MImageWrapper& MImageWrapper::operator=(const MImageWrapper &other)
{
    if (&other != this && other.isValid()) {
        clearImageContainer(mContainer);
        mContainer = copyImageContainer(other.container());
        loadImage();
    }
    return *this;
}

void MImageWrapper::loadImage()
{
    if (mContainer && !mContainer->image && mContainer->name) {
        // get path
        const char* name = mContainer->name;

        // load image
        Resource resource(name);
        if (resource.image()) {
            QImage image = resource.image()->rgbSwapped();

            // new image container
            ImageContainer *tmp = createImageContainer(name,image.byteCount());
            if (tmp) {
                tmp->height = image.height();
                tmp->width  = image.width();
                memmove(tmp->image,image.bits(),tmp->size);

                // swap containers
                clearImageContainer(mContainer);
                mContainer = tmp;
            }
        }
    }
}

MImageWrapper::~MImageWrapper() {
    clearImageContainer(mContainer);
}

bool MImageWrapper::isValid() const {
    return mContainer && mContainer->image && mContainer->size > 0;
}

const ImageContainer* MImageWrapper::container() const {
    return mContainer;
}

const Buffer* MImageWrapper::toBuffer() const {
    if (!mBuffer && isValid()) {
        mBuffer = writeImageContainerToBuffer(mContainer);
    }
    return mBuffer;
}

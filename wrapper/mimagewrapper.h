//
//  MImage.h
//  RPIMultimediaController
//
//  Created by Zoltan Meszaros on 1/18/15.
//  Copyright (c) 2015 Zoltan Meszaros. All rights reserved.
//

#ifndef MIMAGEWRAPPER_H
#define MIMAGEWRAPPER_H

#include <QString>
#include <QImage>
#include "./mabstractwrapper.h"
#include "./../resource.h"
#include "./common/mimage.h"

class MImageWrapper : public MAbstractWrapper {
public:
    MImageWrapper(Buffer *buffer);
    MImageWrapper(const ImageContainer &cont);
    MImageWrapper(const MImageWrapper &other);
    MImageWrapper& operator = (const MImageWrapper &other);
    ~MImageWrapper();

    bool isValid() const;    
    const ImageContainer* container() const;
    const Buffer* toBuffer() const;
private:
    ImageContainer *mContainer;
    void loadImage();
};

#endif // MIMAGEWRAPPER_H

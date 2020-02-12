//
//  MMessage.h
//  RPIMultimediaController
//
//  Created by Zoltan Meszaros on 1/27/15.
//  Copyright (c) 2015 Zoltan Meszaros. All rights reserved.
//

#ifndef __RPIMultimediaController__MMessage__
#define __RPIMultimediaController__MMessage__

#include "mcommon.h"

typedef struct MessageContainer {
    char *message;
} MessageContainer;

MessageContainer* readMessageContainerFromBuffer(Buffer *buffer);
Buffer* writeMessageContainerToBuffer(MessageContainer *cont);

MessageContainer* createMessageContainer();
MessageContainer* clearMessageContainer(MessageContainer *cont);
MessageContainer* copyMessageContainer(const MessageContainer* const cont);

#ifdef QT
#include <QMetaType>
Q_DECLARE_METATYPE(MessageContainer);
#endif

#endif /* defined(__RPIMultimediaController__MMessage__) */

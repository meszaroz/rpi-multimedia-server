//
//  MCommand.h
//  RPIMultimediaController
//
//  Created by Zoltan Meszaros on 1/25/15.
//  Copyright (c) 2015 Zoltan Meszaros. All rights reserved.
//

#ifndef __RPIMultimediaController__MCommand__
#define __RPIMultimediaController__MCommand__

#include "mcommon.h"

typedef enum ECommand {
    Play,
    Pause,
    Stop,
    Seek,
    Volume,
    Undefined
} ECommand;

typedef struct CommandContainer {
    ECommand cmd;
    char    *str;
    TValue  *val;
} CommandContainer;

CommandContainer* readCommandContainerFromBuffer(Buffer *buffer);
Buffer* writeCommandContainerToBuffer(CommandContainer *cont);

CommandContainer* createCommandContainer();
CommandContainer* clearCommandContainer(CommandContainer *cont);
CommandContainer* copyCommandContainer(const CommandContainer* const cont);

#ifdef QT
#include <QMetaType>
Q_DECLARE_METATYPE(CommandContainer);
#endif

#endif /* defined(__RPIMultimediaController__MCommand__) */

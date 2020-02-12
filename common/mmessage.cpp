//
//  MMessage.c
//  RPIMultimediaController
//
//  Created by Zoltan Meszaros on 1/27/15.
//  Copyright (c) 2015 Zoltan Meszaros. All rights reserved.
//

#include "mmessage.h"

static void initMessageContainer(MessageContainer *cont) {
    if (cont) {
        cont->message = 0;
    }
}

static void clearMessage(MessageContainer *cont) {
    if (cont) {
        free(cont->message);
    }
    initMessageContainer(cont);
}

static TSize containerSize(const MessageContainer* const cont) {
    TSize out = 0;
    if (cont && cont->message) {
        out += strlen(cont->message) + 1;
    }
    return out;
}

static bool writeDataToBuffer(Buffer *buffer, MessageContainer *cont) {
    bool out = buffer && cont && isBufferAtBeginning(buffer);
    if (out && cont->message) {
        out = writeDataString(buffer, cont->message) && out;
    }
    return out;
}

MessageContainer* readMessageContainerFromBuffer(Buffer *buffer) {
    MessageContainer *out = buffer && isBufferAtBeginning(buffer) ?
       createMessageContainer() :
       0;
    if (out) {
        out->message = readDataString(buffer);
    }
    return out;
}

Buffer* writeMessageContainerToBuffer(MessageContainer *cont) {
    Buffer *out = 0;
    TSize size = containerSize(cont);
    out = createBuffer(Status,size);
    if (out) {
        writeDataToBuffer(out,cont);
    }
    return out;
}

MessageContainer* clearMessageContainer(MessageContainer *cont) {
    if (cont) {
        clearMessage(cont);
        free(cont);
        cont = 0;
    }
    return cont;
}

MessageContainer* createMessageContainer() {
    MessageContainer *out = (MessageContainer*)malloc(sizeof(MessageContainer));
    if (out) {
        initMessageContainer(out);
    }
    return out;
}

MessageContainer* copyMessageContainer(const MessageContainer* const cont) {
    MessageContainer *out = cont ?
        createMessageContainer() :
        0;
    if (out) {
        copyString(&out->message,cont->message);
    }
    return out;
}

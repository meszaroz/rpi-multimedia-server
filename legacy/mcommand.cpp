//
//  MCommand.c
//  RPIMultimediaController
//
//  Created by Zoltan Meszaros on 1/25/15.
//  Copyright (c) 2015 Zoltan Meszaros. All rights reserved.
//

#include "mcommand.h"

static void initCommandContainer(CommandContainer *cont) {
    if (cont) {
        cont->cmd = Undefined;
        cont->str = 0;
        cont->val = 0;
    }
}

static void clearCommand(CommandContainer *cont) {
    if (cont && cont->cmd < Undefined) {
        free(cont->str);
        free(cont->val);
    }
    initCommandContainer(cont);
}

ECommand readDataCommand(Buffer *buffer) {
    TGeneral tmp = readDataInt(buffer,sizeof(TCommand));
    return tmp != -1 ? (ECommand)tmp : Undefined;
}

static TSize containerSize(const CommandContainer* const cont) {
    TSize out = 0;
    if (cont) {
        out += sizeof(TCommand);
        // string has higher precedence over value
        /**/ if (cont->str) {
            out += strlen(cont->str)+1;
        }
        else if (cont->val) {
            out += sizeof(TValue);
        }
    }
    return out;
}

static bool doesBufferEndWithTerminatingCharacter(Buffer *buffer) {
    return buffer && buffer->size > 0 && *(char*)(buffer->data + buffer->size-1) == '\0';
}

static bool writeDataToBuffer(Buffer *buffer, CommandContainer *cont) {
    bool out = buffer && cont && cont->cmd < Undefined && isBufferAtBeginning(buffer);
    if (out) {
        out = writeDataWithSize(buffer,(TPointer*)(&cont->cmd), sizeof(TCommand))  && out;
        // string has higher precedence over value
        /**/ if (cont->str) {
            out = writeDataString(buffer,cont->str)                                && out;
        }
        else if (cont->val) {
            out = writeDataWithSize(buffer,(TPointer*)(cont->val), sizeof(TValue)) && out;
        }
    }
    return out;
}

CommandContainer* readCommandContainerFromBuffer(Buffer *buffer) {
    CommandContainer *out = canExtractDataWithSize(buffer,sizeof(TCommand)) && isBufferAtBeginning(buffer) ?
                                createCommandContainer() :
                                0;
    if (out) {
        out->cmd = readDataCommand(buffer);
        if (!isBufferAtEnd(buffer)) {
            /**/ if (doesBufferEndWithTerminatingCharacter(buffer)) {
                out->str = readDataString(buffer);
            }
            else if (canExtractDataWithSize(buffer,sizeof(TValue))) {
                out->val = (TValue*)readDataWithSize(buffer,sizeof(TValue));
            }
        }
    }
    return out;
}

Buffer* writeCommandContainerToBuffer(CommandContainer *cont) {
    Buffer *out = 0;
    TSize size = containerSize(cont);
    if (size > 0) {
        out = createBuffer(Command,size);
        if (out) {
            writeDataToBuffer(out,cont);
        }
    }
    return out;
}

CommandContainer* clearCommandContainer(CommandContainer *cont) {
    if (cont) {
        clearCommand(cont);
        free(cont);
        cont = 0;
    }
    return cont;
}

CommandContainer* createCommandContainer() {
    CommandContainer *out = (CommandContainer*)malloc(sizeof(CommandContainer));
    if (out) {
        initCommandContainer(out);
    }
    return out;
}

CommandContainer* copyCommandContainer(const CommandContainer* const cont) {
    CommandContainer *out = cont ?
        createCommandContainer() :
        0;
    if (out) {
        out->cmd = cont->cmd;
        *out->val = *cont->val;
        copyString(&out->str,cont->str);
    }
    return out;
}

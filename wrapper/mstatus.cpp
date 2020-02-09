//
//  MStatus.c
//  RPIMultimediaController
//
//  Created by Zoltan Meszaros on 1/27/15.
//  Copyright (c) 2015 Zoltan Meszaros. All rights reserved.
//

#include "mstatus.h"

static void initStatusContainer(StatusContainer *stat) {
    if (stat) {
        stat->act  = 0;
        stat->play = 0;
        stat->dura = -1;
        stat->pos  = -1;
        stat->vol  = -1;
    }
}

static void clearStatus(StatusContainer *stat) {
    if (stat)
        free(stat->act);
    initStatusContainer(stat);
}

static TSize containerSize(const StatusContainer* const stat) {
    TSize out = 0;
    if (stat) {
        out += stat->act ? strlen(stat->act)+1 : sizeof(char);
        out += sizeof(stat->play);
        out += sizeof(stat->dura);
        out += sizeof(stat->pos );
        out += sizeof(stat->vol );
    }
    return out;
}

static bool writeDataToBuffer(Buffer *buffer, StatusContainer *stat) {
    bool out = buffer && stat && isBufferAtBeginning(buffer);
    if (out) {
        out = writeDataString(buffer, stat->act ? stat->act : stat->act) && out;
        out = writeDataWithSize(buffer,(TPointer*)(&stat->play),sizeof(TBool    )) && out;
        out = writeDataWithSize(buffer,(TPointer*)(&stat->dura),sizeof(TDuration)) && out;
        out = writeDataWithSize(buffer,(TPointer*)(&stat->pos ),sizeof(TSeek    )) && out;
        out = writeDataWithSize(buffer,(TPointer*)(&stat->vol ),sizeof(TVolume  )) && out;
    }
    return out;
}

StatusContainer* readStatusContainerFromBuffer(Buffer *buffer) {
    StatusContainer *out = buffer && isBufferAtBeginning(buffer) ?
                                createStatusContainer() :
                                0;
    if (out) {
        out->act  = readDataString(buffer);
        out->play = (TBool    )readDataInt(buffer,sizeof(TBool    ));
        out->dura = (TDuration)readDataInt(buffer,sizeof(TDuration));
        out->pos  = (TSeek    )readDataInt(buffer,sizeof(TSeek    ));
        out->vol  = (TVolume  )readDataInt(buffer,sizeof(TVolume  ));
    }
    return out;
}

Buffer* writeStatusContainerToBuffer(StatusContainer *stat) {
    Buffer *out = 0;
    TSize size = containerSize(stat);
    if (size > 0) {
        out = createBuffer(Status,size);
        if (out)
            writeDataToBuffer(out,stat);
    }
    return out;
}

StatusContainer* createStatusContainer() {
    StatusContainer *out = (StatusContainer*)malloc(sizeof(StatusContainer));
    if (out)
        initStatusContainer(out);
    return out;
}

StatusContainer* clearStatusContainer(StatusContainer *stat)
{
    if (stat) {
        clearStatus(stat);
        free(stat);
        stat = 0;
    }
    return stat;
}
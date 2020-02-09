#include <assert.h>
#include "resource.h"
#include "executer.h"
#include "player.h"
#include "./common/mcommon.h"

#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Audio.h>

Executer::Executer(Player *player, QObject *parent) :
    AbstractExecuter(parent),
    mPlayer(player)
{
    assert(player != nullptr);
}

MStatusWrapper Executer::status() const
{
    return MStatusWrapper(
                mPlayer->mediaName(),
                mPlayer->playing(),
                mPlayer->duration(),
                mPlayer->time(),
                mPlayer->volume());
}

// if buffer empty -> execute at once - checkBuffer
QString Executer::setStatus(const MStatusWrapper &status)
{
    QString out;

    if (status.isValid()) {
        const StatusContainer *cont = status.container();

        // start video if valid and start streaming
        if (mPlayer->setMediaName(cont->act)) {
            // ToDo: start streaming mPlayer->location();
        }

        // set status
        if (mPlayer->hasMediaName()) {
            mPlayer->setTime   (cont->pos );
            mPlayer->setVolume (cont->vol );
            mPlayer->setPlaying(cont->play);
        }
        else {
            out = "File not opened";
        }
    }
    else {
        out = "Status is invalid";
    }

    return out;
}

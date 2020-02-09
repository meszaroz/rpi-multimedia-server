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
            // start stream and set status
            if (startStream(mPlayer->location())) {
                mPlayer->setTime   (cont->pos );
                mPlayer->setVolume (cont->vol );
                mPlayer->setPlaying(cont->play);
            }
            else {
                out = "Unable to start stream";
            }
        }
        else {
            out = "Media not set";
        }
    }
    else {
        out = "Status is invalid";
    }

    // if error, reset player
    if (!out.isEmpty()) {
        mPlayer->reset();
    }

    return out;
}

bool Executer::stopStream()
{
    if (mStreamProcess.state() != QProcess::NotRunning) {
        mStreamProcess.kill();
    }

    return mStreamProcess.state() == QProcess::NotRunning;
}

bool Executer::startStream(const QString &file)
{
    if (stopStream()) {
        mStreamProcess.start("./tools/stream-mp4", QStringList(file));
    }

    return mStreamProcess.state() != QProcess::NotRunning;
}

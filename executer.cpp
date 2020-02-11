#include <assert.h>
#include "resource.h"
#include "executer.h"
#include "player.h"
#include "./common/mcommon.h"

#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Audio.h>

Executer::Executer(Player *player, QObject *parent) :
    AbstractExecuter(parent),
    mPlayer(player),
    mStreamProcess(nullptr)
{
    assert(player != nullptr);
}

Executer::~Executer()
{
    stopStream();
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
    const StatusContainer *cont = status.container();

    if (status.isValid()) {
        if (mPlayer->setMediaName(cont->act)) {
            if (!startStream(mPlayer->location())) {
                out = "Unable to start stream";
            }
        }
        else if (mPlayer->mediaName() != QString(cont->act)) {
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
    // success -> set status
    else {
        mPlayer->setTime   (cont->pos );
        mPlayer->setVolume (cont->vol );
        mPlayer->setPlaying(cont->play);
    }

    return out;
}

void Executer::stopStream()
{
    if (mStreamProcess != nullptr) {
        if (mStreamProcess->state() != QProcess::NotRunning) {
            mStreamProcess->kill();
        }

        delete mStreamProcess;
        mStreamProcess = nullptr;
    }
}

bool Executer::startStream(const QString &file)
{
    stopStream();

    mStreamProcess = new QProcess;
    mStreamProcess->start("./tools/stream-mp4", QStringList(file));
    mStreamProcess->waitForStarted();

    return mStreamProcess->state() != QProcess::NotRunning;
}

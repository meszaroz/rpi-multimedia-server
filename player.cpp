#include "player.h"
#include "resource.h"
#include "ui_player.h"

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Audio.h>

Player::Player(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Player),
    mMedia(0)
{
    mInstance = new VlcInstance(VlcCommon::args(), this);
    mPlayer = new VlcMediaPlayer(mInstance);
    mPlayer->setVideoWidget(ui->video);
}

Player::~Player()
{
    delete mPlayer;
    delete mMedia;
    delete mInstance;
    delete ui;
}

QString Player::mediaName() const
{
    return mMediaName;
}

bool Player::hasMediaName() const {
    return !mediaName().isEmpty();
}

bool Player::setMediaName(const QString &mediaName)
{
    Resource resource(mediaName);
    bool out = this->mediaName() != mediaName
       && resource.file() && !resource.file()->absoluteFilePath().isEmpty();

    if (out) {
        mMediaName = mediaName;

        // open new media
        VlcMedia *media = new VlcMedia(resource.file()->absoluteFilePath(), true, mInstance);
        mPlayer->openOnly(media);

        // delete old media and assign new
        delete mMedia;
        mMedia = media;        
    }

    return out;
}

QString Player::location() const
{
    return mMedia ?
       mMedia->currentLocation() :
       QString();
}

int Player::duration() const
{
    return mPlayer->length();
}

int Player::time() const
{
    return mPlayer->time();
}

void Player::setTime(const int &time)
{
    mPlayer->setTime(time);
}

int Player::volume() const
{
    return mPlayer->audio()->volume();
}

void Player::setVolume(const int &volume)
{
    mPlayer->audio()->setVolume(volume);
}

bool Player::playing() const
{
    return mPlayer->state() == Vlc::Playing;
}

void Player::setPlaying(const bool &playing)
{
    playing ?
       mPlayer->play() :
       mPlayer->pause();
}

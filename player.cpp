#include "player.h"
#include "resource.h"

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Audio.h>
#include <VLCQtCore/Video.h>
#include <VLCQtWidgets/WidgetVideo.h>

Player::Player(QWidget *parent) :
    QMainWindow(parent),
    mUi(new UI::Player(this)),
    mMedia(0)
{
    mInstance = new VlcInstance(VlcCommon::args(), this);
    mPlayer = new VlcMediaPlayer(mInstance);
    mPlayer->setVideoWidget(mUi->video());
    mUi->video()->setMediaPlayer(mPlayer);
}

Player::~Player()
{
    delete mUi;
    delete mPlayer;
    delete mMedia;
    delete mInstance;
}

QString Player::mediaName() const
{
    return mMediaName;
}

bool Player::hasMediaName() const
{
    return !mediaName().isEmpty();
}

bool Player::isMediaName(const QString &mediaName) const
{
    return hasMediaName() && this->mediaName() == mediaName;
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

QString Player::mediaId() const
{
    return hasMediaName() ?
       QFileInfo(mediaName()).fileName() :
       QString();
}

void Player::reset()
{
    if (mMedia) {
        mPlayer->stop();
        delete mMedia;
        mMedia = nullptr;
        mMediaName.clear();
    }
}

QString Player::location() const
{
    return mMedia ?
       mMedia->currentLocation() :
       QString();
}

int Player::duration() const
{
    return mMedia ?
       mPlayer->length() :
       0;
}

int Player::time() const
{
    return mMedia ?
       mPlayer->time() :
       0;
}

void Player::setTime(const int &time)
{
    if (mMedia) {
        mPlayer->setTime(time);
    }
}

int Player::volume() const
{
    return mMedia ?
       mPlayer->audio()->volume() :
       0;
}

void Player::setVolume(const int &volume)
{
    if (mMedia) {
        mPlayer->audio()->setVolume(volume);
    }
}

EState Player::state() const
{
    if (mMedia) {
        switch (mPlayer->state()) {
            case Vlc::Idle     : return Idle;
            case Vlc::Opening  :
            case Vlc::Buffering: return Busy;
            case Vlc::Playing  : return Playing;
            case Vlc::Paused   : return Paused;
            case Vlc::Stopped  : return Stopped;
            case Vlc::Ended    : return Ended;
            case Vlc::Error    : return Error;
        }
    }
    return Idle;
}

void Player::setState(const EState &state)
{
    if (mMedia) {
        switch (state) {
            case Playing: mPlayer->play();  break;
            case Paused : mPlayer->pause(); break;
            case Idle   :
            case Stopped:
            case Ended  :
            case Error  : mPlayer->stop();  break;
            case Busy   :                   break;
        }
    }
}

UI::Player::Player(::Player *player)
{
    setupUi(player);
}

UI::Player::~Player()
{
    delete mGridLayout;
    delete mVideo;
    delete mCentralWidget;
}

VlcWidgetVideo *UI::Player::video()
{
    return mVideo;
}

void UI::Player::setupUi(::Player *player)
{
    if (player) {
        mCentralWidget = new QWidget(player);
        mGridLayout = new QGridLayout(mCentralWidget);
        mGridLayout->setSpacing(0);
        mGridLayout->setContentsMargins(0, 0, 0, 0);
        mVideo = new VlcWidgetVideo(mCentralWidget);
        mGridLayout->addWidget(mVideo, 0, 0, 1, 1);
        player->setCentralWidget(mCentralWidget);
    }
}

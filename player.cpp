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

void Player::reset()
{
    if (mMedia) {
        mPlayer->stop();
        delete mMedia;
        mMedia = 0;
        mPlayer->openOnly(mMedia);
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

bool Player::playing() const
{
    return mMedia && mPlayer->state() == Vlc::Playing;
}

void Player::setPlaying(const bool &playing)
{
    if (mMedia) {
        playing ?
           mPlayer->play() :
           mPlayer->pause();
    }
}

UI::Player::Player(::Player *player)
{
    setupUi(player);
}

UI::Player::~Player()
{
    delete mCentralWidget;
    delete mGridLayout;
    delete mVideo;
}

VlcWidgetVideo *UI::Player::video() {
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

#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QGridLayout>

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class VlcWidgetVideo;

class Player : public QMainWindow
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);
    ~Player();

    void reset();

    QString mediaName() const;
    bool hasMediaName() const;
    bool setMediaName(const QString &mediaName);

    QString location() const;
    int duration() const;

    int time() const;
    void setTime(const int &time);

    int volume() const;
    void setVolume(const int &volume);

    bool playing() const;
    void setPlaying(const bool &playing);

signals:

public slots:

private:
    VlcInstance    *mInstance;
    VlcMediaPlayer *mPlayer;
    VlcMedia       *mMedia;
    QString         mMediaName; // resource name -> path is extracted from this

    // UI
    QWidget        *mCentralWidget;
    QGridLayout    *mGridLayout;
    VlcWidgetVideo *mVideo;

    void setupUi();
};

#endif // PLAYER_H

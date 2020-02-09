#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>

namespace Ui {
    class Player;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class Player : public QMainWindow
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);
    ~Player();

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
    Ui::Player *ui;

    VlcInstance    *mInstance;
    VlcMediaPlayer *mPlayer;
    VlcMedia       *mMedia;
    QString         mMediaName; // resource name -> path is extracted from this
};

#endif // PLAYER_H

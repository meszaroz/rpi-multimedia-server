#ifndef EXECUTER_H
#define EXECUTER_H

#include <QObject>
#include <QProcess>
#include "./interface/abstractexecuter.h"

class Player;

class Executer : public AbstractExecuter
{
    Q_OBJECT
public:
    explicit Executer(Player *player, QObject *parent = 0);
    ~Executer();

    MStatusWrapper status() const;

signals:
    void statusUpdate(const MStatusWrapper &status);

public slots:
    QString setStatus(const MStatusWrapper &status);

private:
    Player   *mPlayer;
    QProcess *mStreamProcess;

    void stopStream();
    bool startStream(const QString &mediaName, const QString &file);
};

#endif // EXECUTER_H

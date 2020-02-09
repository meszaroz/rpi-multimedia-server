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

    MStatusWrapper status() const;

signals:
    void statusUpdate(const MStatusWrapper &status);

public slots:
    QString setStatus(const MStatusWrapper &status);

private:
    Player  *mPlayer;
    QProcess mStreamProcess;

    bool stopStream();
    bool startStream(const QString &file);
};

#endif // EXECUTER_H

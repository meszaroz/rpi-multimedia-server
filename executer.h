#ifndef EXECUTER_H
#define EXECUTER_H

#include <QObject>
#include "./interface/abstractexecuter.h"

/* command prios:
  (play, stop -> override all), (pause, seek, vol), status
  */

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
    Player *mPlayer;
};

#endif // EXECUTER_H

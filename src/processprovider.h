#ifndef PROCESSPROVIDER_H
#define PROCESSPROVIDER_H

#include <QObject>
#include <QDBusInterface>
#include <QDebug>

class ProcessProvider : public QObject
{
    Q_OBJECT

public:
    explicit ProcessProvider(QObject *parent = nullptr);

    Q_INVOKABLE bool start(const QString &exec);
    Q_INVOKABLE void showLauncherDBus();
};

#endif // PROCESSPROVIDER_H

#include "processprovider.h"
#include <QProcess>

ProcessProvider::ProcessProvider(QObject *parent)
    : QObject(parent)
{

}

bool ProcessProvider::start(const QString &exec)
{
    QProcess process;
    process.setProgram(exec);
    return process.startDetached();
}

void ProcessProvider::showLauncherDBus()
{
    QDBusInterface iface("me.aren.OdysseusLauncher", "/LauncherViewManager", "", QDBusConnection::sessionBus());
    if (iface.isValid()) {
        QDBusMessage reply = iface.call("showView");
        if (reply.type() == QDBusMessage::ErrorMessage) {
            qWarning() << "Error:" << reply.errorMessage();
        }
    } else {
        qWarning() << "Interface is not valid";
    }
}

#include "systemappmonitor.h"

#include <QFileSystemWatcher>
#include <QRegularExpression>
#include <QDirIterator>
#include <QSettings>
#include <QLocale>

#include <QDebug>

#define SystemApplicationsFolder "/usr/share/applications"

static SystemAppMonitor *SELF = nullptr;

static QByteArray detectDesktopEnvironment()
{
    const QByteArray desktop = qgetenv("XDG_CURRENT_DESKTOP");

    if (!desktop.isEmpty())
        return desktop.toUpper();

    return QByteArray("UNKNOWN");
}

SystemAppMonitor *SystemAppMonitor::self()
{
    if (SELF == nullptr)
        SELF = new SystemAppMonitor;

    return SELF;
}

SystemAppMonitor::SystemAppMonitor(QObject *parent)
    : QObject(parent)
{
    QFileSystemWatcher *watcher = new QFileSystemWatcher(this);
    watcher->addPath(SystemApplicationsFolder);
    connect(watcher, &QFileSystemWatcher::directoryChanged, this, &SystemAppMonitor::refresh);
    refresh();
}

SystemAppMonitor::~SystemAppMonitor()
{
    while (!m_items.isEmpty())
        delete m_items.takeFirst();
}

SystemAppItem *SystemAppMonitor::find(const QString &filePath)
{
    for (SystemAppItem *item : m_items)
        if (item->path == filePath)
            return item;

    return nullptr;
}

void SystemAppMonitor::refresh()
{
    QStringList addedEntries;
    for (SystemAppItem *item : m_items)
        addedEntries.append(item->path);

    QStringList allEntries;
    QDirIterator it(SystemApplicationsFolder, { "*.desktop" }, QDir::NoFilter, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        const QString &filePath = it.next();

        if (!QFile::exists(filePath))
            continue;

        allEntries.append(filePath);
    }

    for (const QString &filePath : allEntries) {
        if (!addedEntries.contains(filePath)) {
            addApplication(filePath);
        }
    }

    for (SystemAppItem *item : m_items) {
        if (!allEntries.contains(item->path)) {
            removeApplication(item);
        }
    }

    emit refreshed();
}

void SystemAppMonitor::addApplication(const QString &filePath)
{
    if (find(filePath))
        return;

    QSettings desktop(filePath, QSettings::IniFormat);
    desktop.setIniCodec("UTF-8");
    desktop.beginGroup("Desktop Entry");

    if (desktop.contains("OnlyShowIn")) {
        const QString &value = desktop.value("OnlyShowIn").toString();
        if (!value.contains(detectDesktopEnvironment(), Qt::CaseInsensitive)) {
            return;
        }
    }

    if (desktop.value("NoDisplay").toBool() ||
        desktop.value("Hidden").toBool()) {
        return;
    }

    QString appName = desktop.value(QString("Name[%1]").arg(QLocale::system().name())).toString();
    QString appExec = desktop.value("Exec").toString();

    if (appName.isEmpty())
        appName = desktop.value("Name").toString();

    appExec.remove(QRegularExpression("%."));
    appExec.remove(QRegularExpression("^\""));
    // appExec.remove(QRegularExpression(" *$"));
    appExec = appExec.simplified();

    SystemAppItem *item = new SystemAppItem;
    item->path = filePath;
    item->name = appName;
    item->genericName = desktop.value("GenericName").toString();
    item->comment = desktop.value("Comment").toString();
    item->iconName = desktop.value("Icon").toString();
    qDebug() << desktop.value("Icon").toString();
    item->startupWMClass = desktop.value("StartupWMClass").toString();
    item->args = appExec.split(" ");

    m_items.append(item);
}

void SystemAppMonitor::removeApplication(SystemAppItem *item)
{
    m_items.removeOne(item);
    item->deleteLater();
}

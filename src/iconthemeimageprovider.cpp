#include "iconthemeimageprovider.h"
#include <QIcon>
#include <QDebug>

IconThemeImageProvider::IconThemeImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap IconThemeImageProvider::requestPixmap(const QString &id, QSize *realSize,
                                              const QSize &requestedSize)
{
    qDebug() << "Requesting image: " << id;
    // Sanitize requested size
    QSize size(requestedSize);
    if (size.width() < 1)
        size.setWidth(1);
    if (size.height() < 1)
        size.setHeight(1);

    // Return real size
    if (realSize)
        *realSize = size;

    // Is it a path?
    if (id.contains(QLatin1Char('/'))) {
        qDebug() << "Getting absolute path: " << id;
        return QPixmap(id).scaled(size);
    }

    // Return icon from theme or fallback to a generic icon
    QIcon icon = QIcon::fromTheme(id);
    if (icon.isNull())
        icon = QIcon::fromTheme(QLatin1String("application-x-desktop"));
    return icon.pixmap(size);
}

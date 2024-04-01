#include "popuptips.h"
#include "xwindowinterface.h"

#include <QPainterPath>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQuickItem>
#include <QQmlEngine>

#include <QDebug>

PopupTips::PopupTips(QQuickView *parent)
    : QQuickView(parent)
{
    setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus | Qt::ToolTip);
    setResizeMode(QQuickView::SizeViewToRootObject);
    setClearBeforeRendering(true);
    setDefaultAlphaBuffer(true);
    setColor(Qt::transparent);
    setSource(QUrl(QStringLiteral("qrc:/qml/PopupTips.qml")));
    setPadding(10);
}

void PopupTips::popup(const QPointF point, const QString &text)
{
    if (text.isEmpty()) {
        return hide();
    }

    QMetaObject::invokeMethod(rootObject(), "setText", Q_ARG(QVariant, text));
    QMetaObject::invokeMethod(rootObject(), "setVisible", Q_ARG(QVariant, true));

    setVisible(true);
    setX(point.x());
    setY(point.y() - height() - padding);

    QTimer::singleShot(100, this, &PopupTips::updateBlurRegion);
}

void PopupTips::hide()
{
    setVisible(false);
}

void PopupTips::updateBlurRegion()
{
    QPainterPath path;
    path.addRoundedRect(QRect(0, 0, geometry().width(), geometry().height()), 4, 4);
    XWindowInterface::instance()->enableBlurBehind(this, true, path.toFillPolygon().toPolygon());
}

void PopupTips::setPadding(int padding)
{
    this->padding = padding;
}

int PopupTips::getPadding()
{
    return this->padding;
}

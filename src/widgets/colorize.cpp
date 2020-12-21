#include "colorize.h"

QIcon colorizeIcon(QString path, QColor color) {
    QPixmap pixmap = QPixmap(path);
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.setBrush(color);
    painter.setPen(color);
    painter.drawRect(pixmap.rect());
    QIcon icon = QIcon(pixmap);
    return icon;
}

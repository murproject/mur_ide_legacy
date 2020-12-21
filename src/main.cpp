#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QStyleFactory>
#include <QPrinterInfo>

void makeDark(QApplication &a)
{
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(15,15,15));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);

    palette.setColor(QPalette::Highlight, QColor(61,142,201).lighter());
    palette.setColor(QPalette::HighlightedText, Qt::black);
    palette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::darkGray);
    a.setStyle(QStyleFactory::create("fusion"));
    a.setPalette(palette);
}

void makeLight(QApplication &a) {

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(253, 246, 227));
    palette.setColor(QPalette::WindowText, QColor(0, 43, 54));
    palette.setColor(QPalette::Base, QColor(253, 246, 227));
    palette.setColor(QPalette::AlternateBase, QColor(238, 232, 213));
    palette.setColor(QPalette::ToolTipBase, QColor(0, 43, 54));
    palette.setColor(QPalette::ToolTipText, QColor(0, 43, 54));
    palette.setColor(QPalette::Text, QColor(101, 123, 131));
    palette.setColor(QPalette::Button, QColor(238, 232, 213));
    palette.setColor(QPalette::ButtonText, QColor(0, 43, 54));
    palette.setColor(QPalette::BrightText, Qt::red);

    palette.setColor(QPalette::Highlight, QColor(238, 232, 213));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    palette.setColor(QPalette::Disabled, QPalette::Text, QColor(101, 123, 131));
    palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(101, 123, 131));
    a.setStyle(QStyleFactory::create("fusion"));
    a.setPalette(palette);

}

int main(int argc, char *argv[])
{
    QStringList printers = QPrinterInfo::availablePrinterNames();
    qDebug() << printers;

    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus, true);

    a.setOrganizationName("RoboticsCenter");
    a.setOrganizationDomain("murproject.com");
    a.setApplicationName("murIDE");

    makeDark(a);

    MainWindow w;
    w.show();

    return a.exec();
}

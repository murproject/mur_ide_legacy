#include "welcomewidget.h"
#include "murgloabal.h"
#include "dispatcher.h"
#include "projectitemwidget.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidgetItem>
#include <QSettings>
#include <QFileInfo>
#include <QPainter>
#include <QDirIterator>
#include <QDebug>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QMessageBox>
#include <QException>
#include "colorize.h"

WelcomeWidget::WelcomeWidget(QWidget *parent) :
    QWidget(parent),
    m_openTreeWidget(new QListWidget),
    m_newTreeWidget(new QListWidget),
    m_newProjectButton(new QPushButton(tr("New project"))),
    m_openProjectButton(new QPushButton(tr("Open project")))
{
    createLayouts();
    createConnections();

}

WelcomeWidget::~WelcomeWidget()
{

}

void WelcomeWidget::loadRecentFiles()
{
    if (m_openTreeWidget->count() > 0) {
        m_openTreeWidget->clear();
    }
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "RoboticsCenter", "murIDE");
    QStringList paths = settings.value("RecentPaths").toStringList();
    QStringList names = settings.value("RecentNames").toStringList();

    if (paths.size() != names.size()) {
        settings.setValue("RecentPaths", QStringList());
        settings.setValue("RecentNames", QStringList());

        return;
    }

    for (int i = 0; i < paths.size(); ++i) {
        QFileInfo info(paths.at(i));

        //! FIXME: IDK WTF is going on here. But it's quickfix.
        if (i >= names.size()) {
            return;
        }

        if (info.exists() && info.isFile()) {
            try {
                generateItems(names.at(i), paths.at(i));
            } catch(QException &e) {
                qDebug() << e.what();
            }
        }
    }
}

void WelcomeWidget::loadExamples()
{

    m_newTreeWidget->clear();
    QDirIterator it(QDir::cleanPath(QCoreApplication::applicationDirPath() +
                                    "/examples/"), QStringList() << "*.mur", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString path = it.next();
        QFile project(path);
        if (!project.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(nullptr, QObject::tr("Project open"), QObject::tr("Unable to open project!"), QMessageBox::Ok);
            return;
        }

        QJsonParseError err;
        QJsonObject prj = QJsonDocument::fromJson(project.readAll(), &err).object();
        if (Q_UNLIKELY(err.error != QJsonParseError::NoError)) {
            QMessageBox::warning(nullptr, QObject::tr("Project open"), QObject::tr("Unable to open project! Invalid project format."), QMessageBox::Ok);
            return;
        }
        QString description = prj["description"].toString();
        generateExamples(description, path);
    }
}

void WelcomeWidget::addCurrentFile(QString path, QString name)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "RoboticsCenter", "murIDE");
    QStringList paths = settings.value("RecentPaths").toStringList();
    QStringList names = settings.value("RecentNames").toStringList();

    paths.removeAll(path);
    names.removeAll(name);

    paths.prepend(path);
    names.prepend(name);

    while (paths.size() > MaxRecentFiles) {
           paths.removeLast();
           names.removeLast();
    }

    settings.setValue("RecentPaths", paths);
    settings.setValue("RecentNames", names);
}

//! QColor(61, 142, 201)
void WelcomeWidget::createLayouts()
{
    QHBoxLayout* mainLayout = new QHBoxLayout;
    QVBoxLayout *newLayout = new QVBoxLayout;
    QVBoxLayout *openLayout = new QVBoxLayout;

    QHBoxLayout* newButtonLayout = new QHBoxLayout;
    QHBoxLayout* openButtonLayout = new QHBoxLayout;
    QLabel *recentLable = new QLabel(tr("Recent projects"));
    QLabel *exampleLable = new QLabel(tr("Examples"));
    m_newProjectButton->setIcon(colorizeIcon(":/misc/images/new.png", QColor(61, 142, 201)));
    m_openProjectButton->setIcon(colorizeIcon(":/misc/images/open.png", QColor(61, 142, 201)));

    QFont font = recentLable->font();
    font.setPixelSize(16);
    recentLable->setFont(font);
    exampleLable->setFont(font);

    newButtonLayout->addWidget(m_newProjectButton);
    newButtonLayout->addStretch();
    openButtonLayout->addWidget(m_openProjectButton);
    openButtonLayout->addStretch();

    newLayout->addLayout(newButtonLayout);
    newLayout->addWidget(exampleLable);
    newLayout->addWidget(m_newTreeWidget);

    openLayout->addLayout(openButtonLayout);
    openLayout->addWidget(recentLable);
    openLayout->addWidget(m_openTreeWidget);

    mainLayout->addLayout(newLayout);
    mainLayout->addLayout(openLayout);
    mainLayout->addStretch();

    QPalette pal(m_newTreeWidget->palette());
    pal.setColor(QPalette::Base, QColor(53, 53, 53));

    m_newTreeWidget->setPalette(pal);
    m_openTreeWidget->setPalette(pal);

    m_openTreeWidget->setStyleSheet("QListWidget {"
                                    "border-top: none;"
                                    "border-left: none;"
                                    "border-bottom: none;"
                                    "border-right: none;"
                                    "}");

    m_newTreeWidget->setStyleSheet("QListWidget {"
                                    "border-top: none;"
                                    "border-left: none;"
                                    "border-bottom: none;"
                                    "border-right: none;"
                                    "}");

    m_newTreeWidget->setAutoFillBackground(true);
    setLayout(mainLayout);
}

void WelcomeWidget::createConnections()
{
    connect(m_newProjectButton, &QPushButton::clicked, [this]() {
        DISPATCHER->action("Action.Source.New")->trigger();
    });
    connect(m_openProjectButton, &QPushButton::clicked, [this]() {
        DISPATCHER->action("Action.Source.Open")->trigger();
    });

    connect(m_openTreeWidget, &QListWidget::itemDoubleClicked, [this](QListWidgetItem* item) {
        DISPATCHER->action("Action.Source.Open")->setData(item->data(Qt::UserRole));
        DISPATCHER->action("Action.Source.Open")->trigger();
    });

    connect(m_openTreeWidget, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
        m_newTreeWidget->clearSelection();
    });

    connect(m_newTreeWidget, &QListWidget::itemClicked, [this](QListWidgetItem* item) {
        m_openTreeWidget->clearSelection();
    });

    connect(m_newTreeWidget, &QListWidget::itemDoubleClicked, [this](QListWidgetItem* item) {
        DISPATCHER->action("Action.Source.Open")->setData(item->data(Qt::UserRole));
        DISPATCHER->action("Action.Source.Open")->trigger();
    });

}

void WelcomeWidget::generateItems(QString name, QString desc)
{
    QListWidgetItem *listWidgetItem = new QListWidgetItem;
    m_openTreeWidget->addItem(listWidgetItem);
    ProjectItemWidget *projectItem = new ProjectItemWidget;
    listWidgetItem->setSizeHint (QSize(projectItem->sizeHint().width(), 62));
    m_openTreeWidget->setItemWidget (listWidgetItem, projectItem);
    projectItem->setName(name);
    listWidgetItem->setData(Qt::UserRole, desc);
    projectItem->setDescription(desc);
}

void WelcomeWidget::generateExamples(QString name, QString desc)
{
    QListWidgetItem *listWidgetItem = new QListWidgetItem;
    m_newTreeWidget->addItem(listWidgetItem);
    ProjectItemWidget *projectItem = new ProjectItemWidget;
    listWidgetItem->setSizeHint (QSize(projectItem->sizeHint().width(), 62));
    m_newTreeWidget->setItemWidget (listWidgetItem, projectItem);
    projectItem->setName(name);
    listWidgetItem->setData(Qt::UserRole, desc);
    projectItem->setDescription(desc);
}


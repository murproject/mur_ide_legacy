#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

class WelcomeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WelcomeWidget(QWidget *parent);
    ~WelcomeWidget();

    void loadRecentFiles();
    void loadExamples();
    void addCurrentFile(QString path, QString name);
private:
    void createLayouts();
    void createConnections();
    void generateItems(QString name, QString desc);
    void generateExamples(QString name, QString desc);

    QListWidget* m_openTreeWidget;
    QListWidget* m_newTreeWidget;
    QPushButton *m_newProjectButton;
    QPushButton* m_openProjectButton;

    const int MaxRecentFiles = 7;
};

#endif // WELCOMEWIDGET_H

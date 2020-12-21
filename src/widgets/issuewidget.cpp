#include "issuewidget.h"

#include <QVBoxLayout>
#include <QDebug>

IssueWidget::IssueWidget(QWidget *parent) :
    QWidget(parent),
    m_text(new QLabel(this)),
    m_image(new QLabel(this))
{
    createLayout();
    m_text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void IssueWidget::setData(int line, QString file, QString desc, int type)
{
    m_name = file;
    m_desc = desc;
    m_line = line;
    m_type = type;

    QString issue = QString("File %1 in line %2 : %3").arg(file).arg(line).arg(desc);
    m_text->setText(issue);

    if (type == 2) {
        QPixmap pix(":/misc/images/warning.png");
        m_image->setPixmap(pix.scaled(QSize(16, 16), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

QString IssueWidget::filename()
{
    return m_name;
}

int IssueWidget::line()
{
    return m_line;
}


void IssueWidget::createLayout()
{
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(m_image);
    layout->addWidget(m_text);

    QPixmap pix(":/misc/images/error.png");
    m_image->setPixmap(pix.scaled(QSize(16, 16), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    setLayout(layout);
}

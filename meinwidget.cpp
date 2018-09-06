#include <QtGui>
// nur notwendig, wenn Qt 5:
#include <QPushButton>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
//Ende Qt 5
#include "meinWidget.h"


meinWidget::meinWidget(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *quit = new QPushButton(tr("Ende"));
    quit->setFont(QFont("Times", 18, QFont::Bold));
    connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));

    QPushButton *startbutton= new QPushButton(tr("Start"));
    startbutton->setFont(QFont("Times",18, QFont::Bold));
    connect(startbutton, SIGNAL(clicked()),this,SLOT(start()));

    QPushButton *stopbutton= new QPushButton(tr("Stop"));
    stopbutton->setFont(QFont("Times",18, QFont::Bold));
    connect(stopbutton, SIGNAL(clicked()),this,SLOT(stop()));

    QPushButton *saver = new QPushButton(tr("Sichern"));
    saver->setFont(QFont("Times", 18, QFont::Bold));
    connect(saver, SIGNAL(clicked()), this, SLOT(saveFile()));

    QPushButton *loader = new QPushButton(tr("Laden"));
    loader->setFont(QFont("Times", 18, QFont::Bold));
    connect(loader, SIGNAL(clicked()), this, SLOT(loadFile()));



    meinZeichenFeld = new zeichenFeld;

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(quit, 0, 1);
    gridLayout->addWidget(startbutton, 0, 0);
    gridLayout->addWidget(stopbutton, 1, 0);
    gridLayout->addWidget(saver, 3, 0);
    gridLayout->addWidget(loader, 4, 0);
    gridLayout->addWidget(meinZeichenFeld, 1, 1, 2, 1);
    gridLayout->setColumnStretch(1, 10);
    setLayout(gridLayout);
}


void meinWidget::start(void)
{
    meinZeichenFeld->start();
}

void meinWidget::stop(void)
{
    meinZeichenFeld->stop();
}

void meinWidget::saveFile(void)
{
    QFileDialog dialog(this);
    QString fileName;
    QFile file;

    dialog.setFileMode(QFileDialog::AnyFile);
    fileName = dialog.getSaveFileName(this,
                                      tr("Speichern als"), ".", tr("Savegame (*.myz)"));

    if (fileName.isNull()==false)
    {
        file.setFileName(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Dateifehler"),
                                 tr("Folgende Datei kann nicht verwendet werden: ") + fileName,QMessageBox::Ok);
        }

        meinZeichenFeld->serialize(file);
        file.close();
        return;
    }
}

void meinWidget::loadFile(void)
{
    QFileDialog dialog(this);
    QString fileName;
    QFile file;

    dialog.setFileMode(QFileDialog::AnyFile);
    fileName = dialog.getOpenFileName(this,
                                      tr("Speichern als"), ".", tr("Savegame (*.myz)"));

    if (fileName.isNull()==false)
    {
        file.setFileName(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, tr("Dateifehler"),
                                 tr("Folgende Datei kann nicht geöffnet werden: ") + fileName,QMessageBox::Ok);
        }

        meinZeichenFeld->deserialize(file);
        file.close();
        return;
    }
}

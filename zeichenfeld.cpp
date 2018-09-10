#include <QPainter>

#include <QtGui>
#include <QKeyEvent>
#include <QMessageBox>
#include "zeichenfeld.h"

#include <stdlib.h>

zeichenFeld::zeichenFeld(QWidget *parent)
    : QWidget(parent)
{
    setPalette(QPalette(QColor(250, 250, 200)));
    setAutoFillBackground(true);
    //setMaximumSize(1024,768);
    setFixedSize(800,600);
    x=300;
    y=100;
    startgame=0;
    phase=0;
    score=0;
    setFocusPolicy(Qt::StrongFocus);

    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

zeichenFeld::~zeichenFeld()
{
    for (vector<struct blockinfo *>::iterator pos=blocks.begin();pos!=blocks.end();pos++)
        delete *pos;
}


void zeichenFeld::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    vector<struct blockinfo *>::iterator pos,start;

    painter.begin( this );

    painter.setBrush(Qt::red);
    painter.drawRect(x,560,40,40);

    QString punkte=QString::number(score);

    painter.drawText(10, 10,"Score: " + punkte);


    if(blocks.size()>1){

        pos=blocks.begin();
        for(;;){

            start=pos;
            pos++;

            if(pos==blocks.end())break;
            (*pos)->posY+=(*pos)->speed;
            painter.setBrush((*pos)->color);
            painter.drawRect((*pos)->posX,(*pos)->posY,(*pos)->width,(*pos)->height);

        }
    }

    //painter.drawRect(100,y,30,30);

    if(startgame){

        score++;
        phase=rand() % 4;

        switch(phase)
        {
        case 0:
            if(rand()% 100 + 1 <33){
            struct blockinfo *block;
            block = new struct blockinfo;
            block->posX=rand() % 800 + 20;
            block->posY=0;
            block->speed=rand() % 20 + 10;
            block->color=Qt::white;
            block->width=25;
            block->height=25;
            blocks.push_back(block);
        }
            //if(blocks.size()>20)phase++;

            break;

        case 1:
            if(rand()% 100 + 1 <33){
            struct blockinfo *block;
            block = new struct blockinfo;
            block->posX=rand() % 800 + 20;
            block->posY=0;
            block->speed=rand() % 25 + 15;
            block->color=Qt::blue;
            block->width=30;
            block->height=30;
            blocks.push_back(block);
        }

            break;

        case 2:
            if(rand()% 100 + 1 <33){
            struct blockinfo *block;
            block = new struct blockinfo;
            block->posX=rand() % 800 + 20;
            block->posY=0;
            block->speed=rand() % 35 + 20;
            block->color=Qt::black;
            block->width=50;
            block->height=50;
            blocks.push_back(block);
        }

            break;

        case 3:
            if(rand()% 100 + 1 <33){
            struct blockinfo *block;
            block = new struct blockinfo;
            block->posX=rand() % 800 + 20;
            block->posY=0;
            block->speed=rand() % 40 + 30;
            block->color=Qt::yellow;
            block->width=10;
            block->height=10;
            blocks.push_back(block);
        }

            break;
        }

    }

    painter.end();

}

void zeichenFeld::mousePressEvent( QMouseEvent* event)
{

    if(event->button()== Qt::LeftButton){
        x-=25;
    }

    if(event->button()== Qt::RightButton){
        x+=25;
    }

    update();
}

void zeichenFeld::keyPressEvent( QKeyEvent *event)
{

    switch(event->key())
    {
    case Qt::Key_Left:
        if(x>0)x=x-25;

        break;
        //update();

    case Qt::Key_Right:
        if(x+40<800)x=x+25;
        break;
        //update();

    }

    update();

}


void zeichenFeld::serialize(QFile &file)
{
    vector<struct blockinfo *>::iterator pos;

    QTextStream out(&file);

    out << "MeinZeichnungsFormat" << endl;

    for (pos=blocks.begin();pos!=blocks.end();pos++)
    {
        out  << "p "
             << (*pos)->color.red() << " " << (*pos)->color.green() << " "
             << (*pos)->color.blue()<< " "<< (*pos)->posX   << " "
             << (*pos)->posY << " " << (*pos)->speed << " " <<(*pos)->width <<" " <<(*pos)->height << " " << x << " " << score  <<endl;
    }
}

void zeichenFeld::deserialize(QFile &file)
{
    struct blockinfo *point;
    char c;
    QString test;
    int red, green, blue, posX, posY, speed;

    QTextStream in(&file);

    in >> test;
    if (test != "MeinZeichnungsFormat")
    {
        QMessageBox::warning(this, tr("Formatfehler"),
                             tr("Das ist keine Zechnungsdatei!"),QMessageBox::Ok);
        return;
    }

    in >> c; // Leerstellen werden vom '>>' Operator 'konmsumiert';
    // Zeilenenden nicht.

    while (in.status() == QTextStream::Ok)
    {
        in >> c;
        if (in.status() == QTextStream::ReadPastEnd) break;

        if (c!='p')
        {
            QMessageBox::warning(this, tr("Objektfehler"),
                                 tr("Folgender Objekttyp ist unbekannt: ") + c,QMessageBox::Ok);
            return;
        }
        point=new struct blockinfo;

        //in >> posX >> posY >> speed;
        in >> red >> green >> blue;
        point->color.setRgb(red,green,blue);
        in >> point->posX;
        in >> point->posY;
        in >> point->speed;
        in >> point->width;
        in >> point->height;
        in >> x;
        in >> score;

        in >> c; // Leerstellen werden vom '>>' Operator 'konmsumiert';
        // Zeilenenden nicht.

        blocks.push_back(point);
    }

    update();
}

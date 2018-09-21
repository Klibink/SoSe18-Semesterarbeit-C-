#include <QPainter>

#include <QtGui>
#include <QKeyEvent>
#include <QMessageBox>
#include "zeichenfeld.h"

#include <stdlib.h> //benötigt für rand();

//Konstructor
zeichenFeld::zeichenFeld(QWidget *parent)
    : QWidget(parent)
{
    setPalette(QPalette(QColor(250, 250, 200)));
    setAutoFillBackground(true);
    //setMaximumSize(1024,768);
    setFixedSize(800,600);
    x=380;
    y=100;
    startgame=0;
    phase=0;
    score=0;
    leben=3;
    //setzt das Zeichenfeld in den Fokus; benötigt, damit Key-Press Event funktioniert
    setFocusPolicy(Qt::StrongFocus);

    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

//Destructor
zeichenFeld::~zeichenFeld()
{
    for (vector<struct blockinfo *>::iterator pos=blocks.begin();pos!=blocks.end();pos++)
        delete *pos;
}


void zeichenFeld::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    vector<struct blockinfo *>::iterator pos;
    int lastscore;

    painter.begin( this );

    //Avatar wird gezeichnet
    painter.setBrush(Qt::BDiagPattern);
    painter.drawRect(x,560,40,40);

    //Score wird gezeichnet
    QString punkte=QString::number(score);
    painter.drawText(10, 10,"Score: " + punkte);

    //Wenn der Spieler alle Leben verliert wird Pause auf 20 gesetzt
    // Erzeugung von "Freeze-Effekt" und Ausgabe des "Game-Over-Screen" mit den erreichten Punkten
    if(gameover>0){
        gameover--;
        QFont font=painter.font();
        font.setPointSize(20);
        painter.setFont(font);
        QString punkte2=QString::number(lastscore);
        painter.drawText(200,300,"Game Over! Du hast " + punkte2 + " Punkte erreicht!");

    }
    else {

    // Zuständig für die Lebensanzeige
    if(leben>=3){

        painter.setBrush(Qt::red);
        painter.drawEllipse(650,10,25,25);
        painter.drawEllipse(700,10,25,25);
        painter.drawEllipse(750,10,25,25);
    }

    else if(leben==2){

        painter.setBrush(Qt::red);
        painter.drawEllipse(650,10,25,25);
        painter.drawEllipse(700,10,25,25);
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(750,10,25,25);
    }

    else if(leben==1){

        painter.setBrush(Qt::red);
        painter.drawEllipse(650,10,25,25);
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(700,10,25,25);
        painter.drawEllipse(750,10,25,25);
    }

    // Wenn Spieler 0 Leben übrig hat wird der Vektor geleert(gelöscht) und die Variablen wieder auf den Ursprungszustand gesetzt
    else if(leben<=0){

        blocks.clear();

        leben=3;
        x=380;

        // Zwischenvariable lastscore wird benötigt, damit der Score am Ende des Spiels nicht 0 anzeigt
        lastscore=score;
        score=0;

        // Pause wird auf 20 gesetzt, da der Timer alle 100ms aktualisiert wird, wird das Spiel 2 Sekunden lang "gefreezet"
        gameover=20;
        pause=0;

}
    // hier werden die fallenden Blöcke gezeichnet, falls der Vektor Inhalt hat
    if(blocks.size()>1)
    {

        pos=blocks.begin();
        for(;;)
        {

            pos++;

            //Schleifenendbedingung
            if(pos==blocks.end())break;
            //Zuständig für das "Bewegen" der Blöcke, indem die Y-Position um die gespeicherte Geschwindigkeit der Blöcke verändert wird
            if(pause==0) (*pos)->posY+=(*pos)->speed;
            else pause--;
            // Setze den Painter auf die gewünschte Füllfarbe
            painter.setBrush((*pos)->color);

            //Prüfung, um welchen Blocktyp es sich handelt
            //Typ Gegner zieht Leben ab
            //Typ Freund(else) fügt Leben hinzu
            if((*pos)->typ=="gegner")
            {
                painter.drawRect((*pos)->posX,(*pos)->posY,(*pos)->width,(*pos)->height);
            }
            else painter.drawEllipse((*pos)->posX,(*pos)->posY,(*pos)->width,(*pos)->height);

            //Zuständig für das Löschen der Blöcke die Spielfläche verlassen
           // if((*pos)->posY>610) blocks.erase(pos);

            //Zuständig für das Überprüfen ob Kollision zwischen Avatar und Blöcken vorhanden ist
            if(((*pos)->posX+(*pos)->width)>=x && (*pos)->posX <=(x+40) && ((*pos)->posY+40)>=560 && (*pos)->posY<=(560+40))
            {

                //Überprüft, ob es sich beim kollidierten Objekt um einen Freund oder einen Gegner handelt
                if((*pos)->typ=="freund")
                {
                    //verhindert, dass die Variable "Score" größer als 3 werden kann
                    if(leben<=2)
                    {
                    //Leben wird erhöht und das mit dem Avatar kollidierte Objekt gelöscht
                    leben++;
                    blocks.erase(pos);

                    pause=100;
                    }
                    else blocks.erase(pos);
                }

                else
                {
                //Leben wird verringert und das mit dem Avatar kollidierte Objekt gelöscht
                blocks.erase(pos);
                leben--;

                pause=100;


                }
            }
        }
    }

    //wird ausgelöst, wenn Spieler Startknopf auslöst
    if(startgame){

        //score wird erhöht, solange das Spiel läuft
        score++;

        //Variable Phase ist eine zufällige Zahl zwischen 0 und 5
        //abhängig von Phase werden verschiedene Blocktypen generiert und an das Ende des Vektors gepusht
        phase=rand() % 6;

        switch(phase)
        {
        case 0:
            //if-Bedingung sorgt für unvorhersehbareren Spielablauf, da nicht alle 100ms ein Block generiert wird
            if(rand()% 100 + 1 <33){
            struct blockinfo *block;
            block = new struct blockinfo;
            block->posX=rand() % 800 + 20;
            block->posY=0;
            block->speed=15;
            block->color=Qt::red;
            block->width=25;
            block->height=25;
            block->typ="gegner";
            blocks.push_back(block);
        }

            break;

        case 1:
            //if-Bedingung sorgt für unvorhersehbareren Spielablauf, da nicht alle 100ms ein Block generiert wird
            if(rand()% 100 + 1 <33){
            struct blockinfo *block;
            block = new struct blockinfo;
            block->posX=rand() % 800 + 20;
            block->posY=0;
            block->speed=20;
            block->color=Qt::blue;
            block->width=30;
            block->height=30;
            block->typ="gegner";
            blocks.push_back(block);
        }

            break;

        case 2:
            //if-Bedingung sorgt für unvorhersehbareren Spielablauf, da nicht alle 100ms ein Block generiert wird
            if(rand()% 100 + 1 <33){
            struct blockinfo *block;
            block = new struct blockinfo;
            block->posX=rand() % 800 + 20;
            block->posY=0;
            block->speed=25;
            block->color=Qt::black;
            block->width=50;
            block->height=50;
            block->typ="gegner";
            blocks.push_back(block);
        }

            break;

        case 3:
            //if-Bedingung sorgt für unvorhersehbareren Spielablauf, da nicht alle 100ms ein Block generiert wird
            if(rand()% 100 + 1 <33){
            struct blockinfo *block;
            block = new struct blockinfo;
            block->posX=rand() % 800 + 20;
            block->posY=0;
            block->speed=35;
            block->color=Qt::yellow;
            block->width=10;
            block->height=10;
            block->typ="gegner";
            blocks.push_back(block);
        }

            break;

        case 4:
            //if-Bedingung sorgt für unvorhersehbareren Spielablauf, da nicht alle 100ms ein Block generiert wird
            if(rand()% 100 + 1 <33){
            struct blockinfo *block;
            block = new struct blockinfo;
            block->posX=rand() % 800 + 20;
            block->posY=0;
            block->speed=45;
            block->color=Qt::cyan;
            block->width=10;
            block->height=30;
            block->typ="gegner";
            blocks.push_back(block);
        }

            break;

        case 5:
            //if-Bedingung sorgt für unvorhersehbareren Spielablauf, da nicht alle 100ms ein Block generiert wird; Chance viel geringer als bei anderen Objekten
            if(rand()% 100 + 1 <3){
            struct blockinfo *block;
            block = new struct blockinfo;
            block->posX=rand() % 800 + 20;
            block->posY=0;
            block->speed=10;
            block->color=Qt::green;
            block->width=15;
            block->height=15;
            block->typ="freund";
            blocks.push_back(block);
        }

            break;
        }

    }

    painter.end();

    }

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

//Zuständig für das Verarbeiten von Befehlen die durch die Tastatur ausgelöst werden
void zeichenFeld::keyPressEvent( QKeyEvent *event)
{

    switch(event->key())
    {
    case Qt::Key_Left:
        if(x>0)x=x-25;

        break;

    case Qt::Key_Right:
        if(x+40<800)x=x+25;
        break;

    }

}

//Funktion, die für das Speichern zuständig ist
void zeichenFeld::serialize(QFile &file)
{
    vector<struct blockinfo *>::iterator pos;

    QTextStream out(&file);

    out << "MeinZeichnungsFormat" << endl;

    for (pos=blocks.begin();pos!=blocks.end();pos++)
    {
        out  << "p "
             << (*pos)->color.red() << " " << (*pos)->color.green() << " "
             << (*pos)->color.blue()<< " " << (*pos)->posX   << " "
             << (*pos)->posY << " " << (*pos)->speed << " " <<(*pos)->width << " " <<(*pos)->height << " " << (*pos)->typ << " " << x << " " << score << " " << leben << endl;
    }
}

//Funktion, die für das Laden zuständig ist
void zeichenFeld::deserialize(QFile &file)
{
    struct blockinfo *bloecke;
    char c;
    QString test;
    int red, green, blue;

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

        bloecke=new struct blockinfo;
        in >> red >> green >> blue;
        bloecke->color.setRgb(red,green,blue);
        in >> bloecke->posX;
        in >> bloecke->posY;
        in >> bloecke->speed;
        in >> bloecke->width;
        in >> bloecke->height;
        in >> bloecke->typ;
        in >> x;
        in >> score;
        in >> leben;

        in >> c; // Leerstellen werden vom '>>' Operator 'konmsumiert';
        // Zeilenenden nicht.

        blocks.push_back(bloecke);
    }

    update();
}

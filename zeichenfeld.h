#include <QWidget>
#include <QFile>
#include <QTimer>
#include <vector>
using namespace std;

#include <QtGui>

//beinhaltet alle benötigten Variablen der fallenden Blöcke
struct blockinfo{
    int posX;
    int posY;
    int speed;
    QColor color;

    int width;
    int height;

    QString typ;
};

class zeichenFeld : public QWidget
{

public:
    zeichenFeld(QWidget *parent = 0);
    ~zeichenFeld();

    //startet das Spiel
    void start(void) { timer->start(100); startgame=1; };
    //beendet/stoppt das Spiel
    void stop(void) { timer->stop(); startgame=0; };

    //speichert den Spielzustand
    void serialize(QFile &file);
    //lädt das Spiel aus Savedatei
    void deserialize(QFile &file);

//Avatar und Spielfeld
private:
    QTimer *timer;

    //Vektor, der alle zu zeichnenden Blöcke speichert(abgesehen vom Avatar)
    vector<struct blockinfo *> blocks;

    //X-Koordinate des Avatars
    int x;

    //Y-Koordinate des Avatars
    int y;

    //wenn startgame==1 werdem dem Vektor neue Blöcke hinzugefügt und der Score erhöht
    int startgame;

    //Phase wird eine zufällige Zahl zugewiesen und abhängig davon wird ein bestimmter Block dem Vektor hinzugefügt
    int phase;

    //Score speichert den Punktestand
    int score;

    //Leben speichert die Leben des Spielers
    int leben;

    //Abhängig vom Wert wird das Spiel für eine bestimmte Zeit pausiert(eingefroren)
    int pause;
    //Abhängig vom Wert wird das Spiel für eine bestimmte Zeit pausiert(eingefroren) und der Gameover-Screen gepaintet
    int gameover;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent( QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);

};

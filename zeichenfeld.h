#include <QWidget>
#include <QFile>
#include <QTimer>
#include <vector>
using namespace std;

#include <QtGui> //<-- neu

struct blockinfo{
    int posX;
    int posY;
    int speed;
    QColor color;
};

class zeichenFeld : public QWidget
{

public:
    zeichenFeld(QWidget *parent = 0);
    ~zeichenFeld();

    void start(void) { timer->start(500); test=1; };
    void stop(void) { timer->stop(); test=0; };

    void serialize(QFile &file);
    void deserialize(QFile &file);


private:
    QTimer *timer;
    vector<struct blockinfo *> blocks;

    int x;
    int y;
    int test;
    int phase;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent( QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);

};

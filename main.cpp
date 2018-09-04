#include <QApplication>
#include "meinWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    meinWidget widget;
    widget.setGeometry(200, 100, 500, 500);
    widget.show();
    return app.exec();
}

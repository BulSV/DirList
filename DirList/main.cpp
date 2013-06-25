#include <QString>
#include <QDir>
#include <QtCore>
#include <QTimer>

#include "parser.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    Parser parse(argc, argv);

    QTimer *t = new QTimer;

    t->setInterval(1);

    QObject::connect(&parse, SIGNAL(finished()), t, SLOT(start()));
    QObject::connect(t, SIGNAL(timeout()), &app, SLOT(quit()));

    parse.applyOptions();


    return app.exec();
}

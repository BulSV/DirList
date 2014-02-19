#include <QString>
#include <QDir>
#include <QtCore>
#include <QTimer>

#include "parser.h"

void outputArgs(int argc, char** argv)
{
    for(int i = 0; i < argc; ++i)
    {
        qDebug() << "\n\nThis is args:\n";
        qDebug() << QString::fromLocal8Bit(argv[i]);
        qDebug() << "\n\n";
    }
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

//    outputArgs(argc, argv);

    Parser parse(argc, argv);

    QTimer *t = new QTimer;

    t->setInterval(1);

    QObject::connect(&parse, SIGNAL(finished()), t, SLOT(start()));
    QObject::connect(t, SIGNAL(timeout()), &app, SLOT(quit()));

    parse.applyOptions();


    return app.exec();
}

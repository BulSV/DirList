#include <QString>
#include <QDir>
#include <QtCore>
#include <iostream>

#include "parser.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    Parser parse(argc, argv);

    parse.applyOptions();

    return app.exec();
}


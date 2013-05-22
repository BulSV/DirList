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

#ifdef DEBUG_
    qDebug() << QString::number(Parser::HELP); //-1
    qDebug() << QString::number(Parser::HELP | Parser::SHOWDIRS); //-1
    qDebug() << QString::number(Parser::HELP | Parser::HIDECONSOLE); //-1
    qDebug() << QString::number(Parser::HELP | Parser::SHOWDIRS | Parser::HIDECONSOLE); //-1
    qDebug() << QString::number(Parser::HELP | Parser::RECURSIVE); //-1
    qDebug() << QString::number(Parser::HELP | Parser::SHOWDIRS | Parser::RECURSIVE); //-1
    qDebug() << QString::number(Parser::HELP | Parser::HIDECONSOLE | Parser::RECURSIVE); //-1
    qDebug() << QString::number(Parser::HELP | Parser::SHOWDIRS | Parser::HIDECONSOLE | Parser::RECURSIVE); //-1
    qDebug() << QString::number(Parser::NONE); //0
    qDebug() << QString::number(Parser::SHOWDIRS); //1
    qDebug() << QString::number(Parser::HIDECONSOLE); //2
    qDebug() << QString::number(Parser::SHOWDIRS | Parser::HIDECONSOLE); //3
    qDebug() << QString::number(Parser::RECURSIVE); //4
    qDebug() << QString::number(Parser::SHOWDIRS | Parser::RECURSIVE); //5
    qDebug() << QString::number(Parser::HIDECONSOLE | Parser::RECURSIVE); //6
    qDebug() << QString::number(Parser::SHOWDIRS | Parser::HIDECONSOLE | Parser::RECURSIVE); //7
#endif

    return app.exec();
}


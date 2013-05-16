#include <QString>
#include <QDir>
#include <QtCore>
#include <iostream>

#include "parser.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    if(QString(argv[1]) == QString("-h"))
    {
        Parser::help();
        return app.exec();
    }

    for(int i = 2; i < argc; ++i)
    {
        if(QString(argv[i]) == QString("-r"))
        {            
            QString s = argv[1];
            Parser::recursive(s);
        }
    }

    QDir dir;

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name | QDir::DirsLast);

    QString path = argv[1];

    dir.cd(path);

    QFileInfoList list = dir.entryInfoList();

    if(argc > 2)
    {
        Parser::parseToFile(list, argc, argv);
    }

    Parser::parseToConsole(list);

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

    return app.exec();
}


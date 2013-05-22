#include "parser.h"
#include <QDir>
#include <iostream>
#include <QString>
#include <QTextStream>
#include <QDebug>

Parser::Parser(int argc, char **argv)
    : itsArgc(argc)
    , itsArgv(argv)
    , itsOptions(NONE)
{
}

void Parser::applyOptions()
{
    for(int i = 1; i < itsArgc; ++i)
    {
        collectorOptions(parseOptions(QString(itsArgv[i])));
    }
#ifdef DEBUG_
    qDebug() << "getCollectedOptions() =" << getCollectedOptions();
    qDebug() << "(int)getCollectedOptions() =" << (int)getCollectedOptions();
#endif
    switcher(getCollectedOptions());
}

void Parser::parseToFile(OPTIONS opt)
{
    QDir dir;

    if(opt.testFlag(SHOWDIRS))
    {
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    }
    else
    {
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    }

    dir.setSorting(QDir::Name | QDir::DirsLast);

    if(!dir.exists(itsArgv[1]))
    {
        std::cout << "EROR: path to list don't exist\n";
        return;
    }

    dir.cd(itsArgv[1]);

    QFileInfoList list = dir.entryInfoList();

    QFile file;
    QFileInfo info;

//    if(!dir.exists(itsArgv[2]))
//    {
//        std::cout << "EROR: path to file list save don't exist\n";
//        return;
//    }

    file.setFileName(itsArgv[2]);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "ERROR opening file:" << file.fileName();
        return;
    }

    if(opt.testFlag(RECURSIVE))
    {
        recursive(list, file);
    }

    info.setFile(file);

    QString p = info.filePath();
    QString f = info.fileName();

    std::cout << "\nSaving file: " << qPrintable(info.fileName()) << " into " << qPrintable(p.remove(p.size() - f.size(), p.size() - 1)) << std::endl;

    file.close();
}

void Parser::parseToConsole(OPTIONS opt)
{
    QDir dir;

    if(opt.testFlag(SHOWDIRS))
    {
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    }
    else
    {
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    }

    dir.setSorting(QDir::Name | QDir::DirsLast);

    if(!dir.exists(itsArgv[1]))
    {
        std::cout << "EROR: path to list don't exist\n";
        return;
    }

    dir.cd(itsArgv[1]);

    QFileInfoList list = dir.entryInfoList();

    if(opt.testFlag(RECURSIVE))
    {
        recursive(list);
    }
}

void Parser::help()
{
    std::cout << "usage:\t[--help]\n";
    std::cout << "   or:\t<path to list> [options]\n";
    std::cout << "   or:\t<path to list> <path and file name to save results> [options]\n";
    std::cout << "options:\n";
    std::cout << "\t-d\tshow directories\n";
    std::cout << "\t-h\thide output to console\n";
    std::cout << "\t-r\trecursively listing\n";
    std::cout << "Ctrl+C to skip\n";
}

void Parser::recursive(QFileInfoList &list)
{    
    for(int iList = 0; iList < list.size(); ++iList)
    {
        QFileInfo fileInfo = list.at(iList);
        QString filePath = fileInfo.absoluteFilePath();

        if(fileInfo.isDir())
        {
            std::cout << std::endl << qPrintable(filePath) << ":" << std::endl;
            recursive(list);
        }
        else
        {
            std::cout << qPrintable(fileInfo.fileName()) << std::endl;
        }
    }
}

void Parser::recursive(QFileInfoList &list, QFile &file)
{
    QTextStream out(&file);

    for(int iList = 0; iList < list.size(); ++iList)
    {
        QFileInfo fileInfo = list.at(iList);
        QString filePath = fileInfo.absoluteFilePath();

        if(fileInfo.isDir())
        {
            out << "\n" << filePath << ":" << "\n";
            recursive(list, file);
        }
        else
        {
            out << qPrintable(fileInfo.fileName()) << "\n";
        }
    }
}

void Parser::switcher(Parser::OPTIONS opt)
{
#ifdef DEBUG_
    qDebug() << "opt =" << QString::number(opt) << opt.testFlag(HELP);
#endif
    if(opt.testFlag(HELP))
    {
#ifdef DEBUG_
    qDebug() << "help!";
#endif
        help();
        return;
    }
    if(!opt.testFlag(HIDECONSOLE))
    {
        parseToConsole(opt);
    }
    // if(save to file)
    parseToFile(opt);
}

Parser::OPTIONS Parser::parseOptions(QString opt)
{
    if(opt == "--help") return HELP;
    if(opt == "-d") return SHOWDIRS;
    if(opt == "-h") return HIDECONSOLE;
    if(opt == "-r") return RECURSIVE;
    if(opt == "-dh" || opt == "-hd") return SHOWDIRS | HIDECONSOLE;
    if(opt == "-dr" || opt == "-rd") return SHOWDIRS | RECURSIVE;
    if(opt == "-hr" || opt == "-rh") return HIDECONSOLE | RECURSIVE;
    if(opt == "-dhr" || opt == "-drh" ||
            opt == "-hdr" || opt == "-hrd" ||
            opt == "-rdh" || opt == "-rhd")
        return SHOWDIRS | HIDECONSOLE | RECURSIVE;

    return NONE;
}

void Parser::collectorOptions(Parser::OPTIONS opt)
{
    itsOptions = itsOptions | opt;
#ifdef DEBUG_
    qDebug() << "itsOptions =" << QString::number(itsOptions);
#endif
}

Parser::OPTIONS Parser::getCollectedOptions()
{
    return itsOptions;
}

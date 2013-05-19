#include "parser.h"
#include <QDir>
#include <iostream>
#include <QString>
#include <QTextStream>
#include <QDebug>


Parser::OPTIONS Parser::itsOptions = NONE;

void Parser::parse(int argc, char **argv)
{
    for(int i = 1; i < argc; ++i)
    {
        collectorOptions(parseOptions(QString(argv[i])));
    }
#ifdef DEBUG
    qDebug() << "getCollectedOptions() =" << getCollectedOptions();
    qDebug() << "(int)getCollectedOptions() =" << (int)getCollectedOptions();
#endif
    switcher(getCollectedOptions());
}

void Parser::parseToFile(QFileInfoList list, int argc, char** argv)
{
    QFile file;
    QFileInfo info;

    file.setFileName(argv[2]);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    for(int i = 2; i < argc; ++i)
    {
        if(QString(argv[i]) == QString("-r"))
        {
            QString s = QString::fromUtf8(argv[1]);
            recursive(s, file);
            file.close();
            return;
        }
    }

    QTextStream out(&file);

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        out << QString("%1").arg(fileInfo.fileName()) << "\n";
    }
    info.setFile(file);

    std::cout << "\nSaving file: " << qPrintable(info.fileName()) << " into " << argv[2] << std::endl;

    file.close();
}

void Parser::parseToConsole(QFileInfoList list)
{
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        std::cout << qPrintable(QString("%1").arg(fileInfo.fileName()));
        std::cout << std::endl;
    }
}

void Parser::help()
{
    std::cout << "DirList [-h]\n";
    std::cout << "-this help\n";
    std::cout << "DirList [path to list] [-r]\n";
    std::cout << "DirList [path to list] [file name to save results] [-r]\n";
    std::cout << "DirList [path to list] [path to file with file name to save results] [-r]\n";
    std::cout << "- -r is recursive option\n";
    std::cout << "Ctrl + C to skip\n";
}

void Parser::recursive(QString &dirPath)
{
    QDir dir(dirPath);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name | QDir::DirsLast);
    QFileInfoList list = dir.entryInfoList();

    for(int iList = 0; iList < list.size(); ++iList)
    {
        QFileInfo fileInfo = list.at(iList);
        QString filePath = fileInfo.absoluteFilePath();

        if(fileInfo.isDir())
        {
            std::cout << std::endl << qPrintable(filePath) << ":" << std::endl;
            recursive(filePath);
        }
        else
        {
            std::cout << qPrintable(fileInfo.fileName()) << std::endl;
        }
    }
}

void Parser::recursive(QString &dirPath, QFile &file)
{
    QDir dir(dirPath);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Name | QDir::DirsLast);
    QTextStream out(&file);
    QFileInfoList list = dir.entryInfoList();

    for(int iList = 0; iList < list.size(); ++iList)
    {
        QFileInfo fileInfo = list.at(iList);
        QString filePath = fileInfo.absoluteFilePath();

        if(fileInfo.isDir())
        {
            out << "\n" << qPrintable(filePath) << ":" << "\n";
            recursive(filePath, file);
        }
        else
        {
            out << qPrintable(fileInfo.fileName()) << "\n";
        }
    }
}

void Parser::switcher(Parser::OPTIONS opt)
{
#ifdef DEBUG0
    qDebug() << "swithcer begin";
#endif
    if(opt.testFlag(HELP))
        help();
    if(opt.testFlag(Parser::NONE) && !opt.testFlag(SHOWDIRS) && !opt.testFlag(HIDECONSOLE) && !opt.testFlag(RECURSIVE))
        qDebug() << "NONE";
    if(opt.testFlag(SHOWDIRS) && !opt.testFlag(HIDECONSOLE) && !opt.testFlag(RECURSIVE))
        qDebug() << "SHOWDIRS";
    if(!opt.testFlag(SHOWDIRS) && opt.testFlag(HIDECONSOLE) && !opt.testFlag(RECURSIVE))
        qDebug() << "HIDECONSOLE";
    if(!opt.testFlag(SHOWDIRS) && !opt.testFlag(HIDECONSOLE) && opt.testFlag(RECURSIVE))
        qDebug() << "RECURSIVE";
    if(opt.testFlag(SHOWDIRS) && opt.testFlag(HIDECONSOLE) && !opt.testFlag(RECURSIVE))
        qDebug() << "SHOWDIRS | HIDECONSOLE";
    if(opt.testFlag(SHOWDIRS) && !opt.testFlag(HIDECONSOLE) && opt.testFlag(RECURSIVE))
        qDebug() << "SHOWDIRS | RECURSIVE";
    if(!opt.testFlag(SHOWDIRS) && opt.testFlag(HIDECONSOLE) && opt.testFlag(RECURSIVE))
        qDebug() << "HIDECONSOLE | RECURSIVE";
    if(opt.testFlag(SHOWDIRS) && opt.testFlag(HIDECONSOLE) && opt.testFlag(RECURSIVE))
        qDebug() << "SHOWDIRS | HIDECONSOLE | RECURSIVE";
    if(!opt.testFlag(NONE) && !opt.testFlag(SHOWDIRS) && !opt.testFlag(HIDECONSOLE) && !opt.testFlag(RECURSIVE))
        qDebug() << "EROR";
#ifdef DEBUG0
    qDebug() << "opt.testFlag(NONE) =" << opt.testFlag(NONE);
    qDebug() << "opt.testFlag(SHOWDIRS) =" << opt.testFlag(SHOWDIRS);
    qDebug() << "opt.testFlag(HIDECONSOLE) =" << opt.testFlag(HIDECONSOLE);
    qDebug() << "opt.testFlag(RECURSIVE) =" << opt.testFlag(RECURSIVE);
    qDebug() << "switcher end";
#endif
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
#ifdef DEBUG0
    qDebug() << "itsOptions =" << QString::number(itsOptions);
#endif
}

Parser::OPTIONS Parser::getCollectedOptions()
{
    return itsOptions;
}

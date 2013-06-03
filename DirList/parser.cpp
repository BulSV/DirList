#include "parser.h"
#include <QFileInfoList>
#include <iostream>
#include <QString>

#ifdef DEBUG
#include <QDebug>
#endif

Parser::Parser(int argc, char **argv)
    : itsArgc(argc)
    , itsArgv(argv)
    , itsDir(new QDir)
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

void Parser::parseToFile()
{    
    itsDir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    itsDir->setSorting(QDir::Name | QDir::DirsLast | QDir::Type);

    if(!itsDir->exists(itsArgv[1]))
    {
        std::cout << "EROR: path to list don't exist\n";
        return;
    }

    itsDir->cd(itsArgv[1]);    

    QFile file;
    QFileInfo info;

    file.setFileName(itsArgv[2]);
    info.setFile(file);    

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        std::cout << "ERROR opening file:" << qPrintable(file.fileName());
        return;
    }

    QTextStream out(&file);
    out << itsArgv[1] << ":" << "\n";

    QString filePath = info.absoluteFilePath();

    if(itsOptions.testFlag(RECURSIVE))
    {
        recursive(filePath, out);
    }
    else
    {
        notRecursive(filePath, out);
    }

    QString p = info.filePath();
    QString n = info.fileName();

    std::cout << "\nSaving file: " << qPrintable(info.fileName())
              << " into "
              << qPrintable(p.remove(p.size() - n.size(), p.size() - 1))
              << std::endl;

    file.close();
}

void Parser::parseToConsole()
{

    itsDir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    itsDir->setSorting(QDir::Name | QDir::DirsLast | QDir::Type);

    if(!itsDir->exists(itsArgv[1]))
    {
        std::cout << "EROR: path to list don't exist\n";
        return;
    }

    itsDir->cd(itsArgv[1]);
    std::cout << itsArgv[1] << ":" << std::endl;

    if(itsOptions.testFlag(RECURSIVE))
    {
        recursive(itsDir->absolutePath());
    }
    else
    {
        notRecursive(itsDir->absolutePath());
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

void Parser::recursive(const QString &dirPath)
{    
    itsDir->cd(dirPath);
    QFileInfoList list = itsDir->entryInfoList();
#ifdef DEBUG_
    qDebug() << "itsDir->entryInfoList().length():" << itsDir->entryInfoList().length();
#endif

    for(int iList = 0; iList < list.size(); ++iList)
    {
        QFileInfo fileInfo = list.at(iList);
        QString filePath = fileInfo.absoluteFilePath();
#ifdef DEBUG_
    qDebug() << "list:" << filePath;
    qDebug() << "list.size:" << list.size();
#endif

        if(fileInfo.isDir())
        {
            if(itsOptions.testFlag(SHOWDIRS))
            {
                std::cout << std::endl << qPrintable(filePath) << ":" << std::endl;
            }
            recursive(filePath);
        }
        else
        {
            std::cout << qPrintable(fileInfo.fileName()) << std::endl;            
        }
    }
}

void Parser::recursive(const QString &dirPath, QTextStream &out)
{    
    itsDir->cd(dirPath);

    QFileInfoList list = itsDir->entryInfoList();

    for(int iList = 0; iList < list.size(); ++iList)
    {
        QFileInfo fileInfo = list.at(iList);
        QString filePath = fileInfo.absoluteFilePath();

        if(fileInfo.isDir())
        {
            if(itsOptions.testFlag(SHOWDIRS))
            {
                out << "\n" << filePath << ":" << "\n";
            }
            recursive(filePath, out);
        }
        else
        {
            out << fileInfo.fileName() << "\n";
        }
    }
}

void Parser::notRecursive(const QString &dirPath)
{
    itsDir->cd(dirPath);

    QFileInfoList list = itsDir->entryInfoList();

    for(int iList = 0; iList < list.size(); ++iList)
    {
        QFileInfo fileInfo = list.at(iList);
        QString filePath = fileInfo.absoluteFilePath();

        if(fileInfo.isDir())
        {
            if(itsOptions.testFlag(SHOWDIRS))
            {
                std::cout << std::endl << qPrintable(filePath) << ":" << std::endl;
            }
        }
        else
        {
            std::cout << qPrintable(fileInfo.fileName()) << std::endl;
        }
    }
}

void Parser::notRecursive(const QString &dirPath, QTextStream &out)
{
    itsDir->cd(dirPath);

    QFileInfoList list = itsDir->entryInfoList();    

    for(int iList = 0; iList < list.size(); ++iList)
    {
        QFileInfo fileInfo = list.at(iList);
        QString filePath = fileInfo.absoluteFilePath();

        if(fileInfo.isDir())
        {
            if(itsOptions.testFlag(SHOWDIRS))
            {
                out << "\n" << filePath << ":" << "\n";
            }
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
        parseToConsole();
    }

    // if(save to file)
    if(itsDir->isAbsolutePath(itsArgv[2]))
    {
        parseToFile();
    }
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

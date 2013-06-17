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
    parseOptions();
#ifdef DEBUG
    qDebug() << "getCollectedOptions() =" << getCollectedOptions();
    qDebug() << "(int)getCollectedOptions() =" << (int)getCollectedOptions();
#endif
    switcher(getCollectedOptions());
}

void Parser::parseToFile()
{    
    if(itsOptions.testFlag(HIDENFILES))
    {
        itsDir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    }
    else
    {
        itsDir->setFilter(QDir::Files | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    }
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

    if(itsOptions.testFlag(HIDENFILES))
    {
        itsDir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    }
    else
    {
        itsDir->setFilter(QDir::Files | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot);
    }
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
    qDebug() << "usage:\t[--help]\n";
    qDebug() << "   or:\t<path to list> [options]\n";
    qDebug() << "   or:\t<path to list> <path and file name to save results> [options]\n";
    qDebug() << "options:\n";
    qDebug() << "\t-d\tshow directories\n";
    qDebug() << "\t-h\thide output to console\n";
    qDebug() << "\t-r\trecursively listing\n";
    qDebug() << "Ctrl+C to skip\n";
}

void Parser::recursive(const QString &dirPath)
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
                if(itsOptions.testFlag(ABSOLUTEPATH))
                {
                    QString temp = filePath;
                    std::cout << std::endl << "." << qPrintable(temp.remove(0, QString(itsArgv[1]).size())) << ":" << std::endl;
                }
                else
                {
                    std::cout << std::endl << qPrintable(filePath) << ":" << std::endl;
                }
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
                if(itsOptions.testFlag(ABSOLUTEPATH))
                {
                    QString temp = filePath;
                    out << "\n" << "." << temp.remove(0, QString(itsArgv[1]).size()) << ":\n";
                }
                else
                {
                    out << "\n" << filePath << ":" << "\n";
                }
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
    if(opt.testFlag(HELP))
    {
        help();
        return;
    }
    if(!opt.testFlag(HIDECONSOLE))
    {
        parseToConsole();
    }

    // if save to file
    if(itsDir->isAbsolutePath(itsArgv[2]))
    {
        parseToFile();
    }
}

Parser::OPTIONS Parser::parseOptions()
{    
    QString str;
    for(int i = 1; i < itsArgc; ++i)
    {
        str.append(QString(itsArgv[i]));
    }
#ifdef DEBUG
    qDebug() << "str:" << str;
#endif

    QVector <QStringList> opts = polyOptParser(str);

    if(opts.at(0).contains("help")) collectorOptions(HELP);
    if(opts.at(1).contains("d")) collectorOptions(SHOWDIRS);
    if(opts.at(1).contains("h")) collectorOptions(HIDECONSOLE);
    if(opts.at(1).contains("r")) collectorOptions(RECURSIVE);
    if(opts.at(1).contains("s")) collectorOptions(HIDENFILES);
    if(opts.at(1).contains("a")) collectorOptions(ABSOLUTEPATH);

    if(!opts.at(2).isEmpty())
    {
        itsDir->setNameFilters(opts.at(2));
#ifdef DEBUG
        qDebug() << "opts.at(2)" << opts.at(2);
#endif
    }
    if(!opts.at(3).isEmpty())
    {
        itsDir->setNameFilters(opts.at(3));
#ifdef DEBUG
        qDebug() << "opts.at(3)" << opts.at(3);
#endif
    }

    return getCollectedOptions();
}

void Parser::collectorOptions(Parser::OPTIONS opt)
{
    itsOptions = itsOptions | opt;
#ifdef DEBUG
    qDebug() << "itsOptions =" << QString::number(itsOptions);
#endif
}

Parser::OPTIONS Parser::getCollectedOptions() const
{
    return itsOptions;
}

QVector <QStringList> Parser::polyOptParser(QString str)
{
    QString ext = QString("(\\*\\.\\w+,?\\s?)");
    QString f = QString("(\\^\\w+,?\\s?)");
    QString opt_opt = QString("(-[hrdsa]+[^elp]+)");
    QString opt_help = QString("(--help)");
    QString opt_ext = QString("(--ext:\"%1+\")").arg(ext);
    QString opt_f = QString("(--f:\"%1+\")").arg(f);
    QString opt = QString("%1?\\s?%2?\\s?%3?%4?\\s?").arg(opt_help).arg(opt_opt).arg(opt_ext).arg(opt_f);

    QRegExp rx_ext(ext);
    QRegExp rx_f(f);
    QRegExp rx_opt_opt(opt_opt);
    QRegExp rx_opt_ext(opt_ext);
    QRegExp rx_opt_f(opt_f);
    QRegExp rx_opt_help(opt_help);
    QRegExp rx_opt(opt);

    QStringList listOpt;
    QStringList listExt;
    QStringList listFiles;
    QStringList listHelp;

    int pos = 0;

    QString stringExt;
    QString stringFiles;
    QString stringOpt;
    QString stringHelp;

    if((rx_opt.indexIn(str, 0) != -1) && (rx_opt_help.indexIn(str, 0) == -1))
    {
        if(rx_opt_opt.indexIn(str, 0) != -1)
        {
            while((pos = rx_opt_opt.indexIn(str, pos)) != -1)
            {
                stringOpt = rx_opt_opt.cap(1);
                listOpt.append(stringOpt);
                pos += rx_opt_opt.matchedLength();
            }
        }
        if(rx_opt_ext.indexIn(str, 0) !=-1)
        {
            pos = rx_opt_ext.indexIn(str, 0);

            while (((pos = rx_ext.indexIn(str, pos)) != -1) &&
                   ((pos < rx_opt_f.indexIn(str, 0)) || (rx_opt_f.indexIn(str, 0) == -1)))
            {
                stringExt = rx_ext.cap(1);
                listExt.append(stringExt);
                pos += rx_ext.matchedLength();
            }
        }
        if(rx_opt_f.indexIn(str, 0) !=-1)
        {
            pos = rx_opt_f.indexIn(str, 0);
            while ((pos = rx_f.indexIn(str, pos)) != -1)
            {
                stringFiles = rx_f.cap(1);
                listFiles.append(stringFiles);
                pos += rx_f.matchedLength();
            }
        }
    }
    else if((rx_opt_help.indexIn(str, 0) != -1) && (rx_opt.indexIn(str, 0) != -1))
    {
        stringHelp = rx_opt_help.cap(0);
        listHelp.append(stringHelp);
    }
    else
    {
        qDebug() << "Incorrect input options!";
    }

    QStringList sl;

    foreach (QString temp_str, listOpt) {
        sl.append(temp_str.replace(QRegExp("-"), "").split(QRegExp("(?=\\w)"), QString::SkipEmptyParts));
    }
    listOpt = sl;
    sl.clear();

    listOpt.removeDuplicates();

    if(!listOpt.isEmpty())
        qDebug() << "\nOptions:\n" << listOpt << "\n";
    else
        qDebug() << "\nEmpty Options\n";

    foreach (QString temp_str, listExt) {
        sl.append(temp_str.replace(QRegExp("[\\s,]"), ""));
    }
    listExt = sl;
    sl.clear();

    listExt.removeDuplicates();

    if(!listExt.isEmpty())
        qDebug() << "\nMask Extensions:\n" << listExt << "\n";
    else
        qDebug() << "\nEmpty Mask Extentions\n";

    foreach (QString temp_str, listFiles) {
        sl.append(temp_str.replace(QRegExp("[\\^\\s,]"), ""));
    }
    listFiles = sl;
    sl.clear();

    listFiles.removeDuplicates();

    if(!listFiles.isEmpty())
        qDebug() << "\nFiles Names:\n" << listFiles << "\n";
    else
        qDebug() << "\nEmpty Files Names\n";

    foreach (QString temp_str, listHelp)
    {
        sl.append(temp_str.remove(QRegExp("--")));
    }
    listHelp = sl;
    sl.clear();

    listHelp.removeDuplicates();

    if(!listHelp.isEmpty())
        qDebug() << "\nHelp:\n" << listHelp << "\n";
    else
        qDebug() << "\nEmpty Help\n";

    QVector <QStringList> list;

    list.append(listHelp); // 0
    list.append(listOpt);  // 1
    list.append(listExt);  // 2
    list.append(listFiles);// 3

#ifdef DEBUG
    qDebug() << "list.at(listHelp)" << list[0];
    qDebug() << "list.at(listOpt)" << list[1];
    qDebug() << "list.at(listExt)" << list[2];
    qDebug() << "list.at(listFiles)" << list[3];
#endif

    return list;
}

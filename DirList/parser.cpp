#include "parser.h"
#include <QFileInfoList>
#include <QString>
#include <QDebug>

#define ONESYMBOL 100
#define SYMBOLBLOCK 2000

Parser::Parser(int argc, char **argv, QObject *parent)
    : QObject(parent)
    , itsArgc(argc)
    , itsArgv(argv)
    , itsDir(new QDir)
    , itsOptions(NONE)
    , itsFilters(QStringList() << "")    
{    
}

void Parser::applyOptions()
{
#ifdef DEBUG
    qDebug() << "getCollectedOptions() =" << getCollectedOptions();
    qDebug() << "(int)getCollectedOptions() =" << (int)getCollectedOptions();
#endif
    switcher(parseOptions());

    emit finished();
}

void Parser::parseToFile()
{    
    dirFilters();

#ifdef Q_OS_LINUX
    if(!itsDir->exists(itsArgv[1]))
#else
    if(!itsDir->exists(QString::fromLocal8Bit(itsArgv[1])))
#endif
    {
        qCritical() << "EROR: path to list don't exist\n";
        return;
    }

#ifdef Q_OS_LINUX
    itsDir->cd(itsArgv[1]);
#else
    itsDir->cd(QString::fromLocal8Bit(itsArgv[1]));
#endif
    QFile file;
    QFileInfo info;

#ifdef Q_OS_LINUX
    file.setFileName(itsArgv[2]);
#else
    file.setFileName(QString::fromLocal8Bit(itsArgv[2]));
#endif
    info.setFile(file);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qCritical() << "ERROR opening file:" << file.fileName();
        return;
    }

    QTextStream out(&file);
#ifdef Q_OS_LINUX
    out << "ROOT DIR:\n" << itsArgv[1] << ":" << "\n";
#else
    out << "ROOT DIR:\n" << QString::fromLocal8Bit(itsArgv[1]) << ":" << "\n";
#endif    

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

    QTextStream itsOut(stdout);

    itsOut << "\nSaving file: " << info.fileName()
           << " into "
           << p.remove(p.size() - n.size(), p.size() - 1)
           << "\n";

    file.close();
}

void Parser::parseToConsole()
{

    dirFilters();

#ifdef Q_OS_LINUX
    if(!itsDir->exists(itsArgv[1]))
#else
    if(!itsDir->exists(QString::fromLocal8Bit(itsArgv[1])))
#endif
    {
        qCritical() << "EROR: path to list don't exist\n";
        return;
    }

    QTextStream itsOut(stdout);

#ifdef Q_OS_LINUX
    itsDir->cd(itsArgv[1]);
    itsOut << "ROOT DIR:\n" << itsArgv[1] << ":\n";
#else
    itsDir->cd(QString::fromLocal8Bit(itsArgv[1]));
    itsOut << "ROOT DIR:\n" << QString::fromLocal8Bit(itsArgv[1]) << ":\n";
    itsOut.flush(); // чтобы ROOT DIR выводился в начале, а не в конце
#endif

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
    QTextStream itsOut(stdout);

    itsOut << "usage:\t[--help]\tthis help\n";
    itsOut << "   or:\t<path to list> [options]\n";
    itsOut << "   or:\t<path to list> <path and file name to save results> [options]\n";
    itsOut << "\noptions:\n";
    itsOut << "\t-d\tshow directories\n";
    itsOut << "\t-h\thide output to console\n";
    itsOut << "\t-r\trecursively listing\n";
    itsOut << "\t-s\tshow hidden files\n";
    itsOut << "\t-a\tlist absolute paths\n";
    itsOut << "\t--ext:\"*.ext1, *.ext2,*.ext3 *.ext4\"\n\t\tcreate extension mask on listing files\n";
    itsOut << "\t--f:\"^fileName1, ^fileName2,^fileName3 ^fileName4\"\n\t\tcreate file names mask on listing files or dirs\n\t\tit understands \"*\" and \"?\" wildcards\n";
    itsOut << "\nPress Ctrl+C to skip\n";
}

void Parser::dirFilters()
{
    if(itsOptions.testFlag(HIDENFILES))
    {
        itsDir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot | QDir::AllDirs);
    }
    else
    {
        itsDir->setFilter(QDir::Files | QDir::NoSymLinks | QDir::Dirs | QDir::NoDotAndDotDot | QDir::AllDirs);
    }
    itsDir->setSorting(QDir::Name | QDir::DirsLast | QDir::Type);
}

void Parser::recursive(const QString &dirPath)
{    
    itsDir->cd(dirPath);

    dirFilters();

    QFileInfoList list = itsDir->entryInfoList();
    QTextStream itsOut(stdout);

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
                    itsOut << "\nSUBDIR:\n" << QDir::toNativeSeparators(filePath) << ":\n";
                }
                else
                {
                    QString temp = filePath;
                    itsOut << "\nSUBDIR:\n.." << QDir::toNativeSeparators(temp.remove(0, QString(itsArgv[1]).size())) << ":\n";
                }
            }
            itsOut.flush(); // чтобы после ROOT DIR:\nпуть к корневой папке\n было "..ке\n", т.е. сробатывл перевод строки
                            // + к тому же, чтобы, если там будут нужные для вывода файлы, они выводились в правильном порядке
            recursive(filePath);            
        }
        else
        {
            itsOut << fileInfo.fileName() << "\n";
        }
    }
}

void Parser::wait()
{
    if(itsOptions.testFlag(HIDECONSOLE))
    {
        QTextStream itsOut(stdout);
        static int i = 0;
        i++;
        itsOut.flush();
        if(!(i % ONESYMBOL))
            itsOut <<"=";
        if (!(i % SYMBOLBLOCK))
        {
            itsOut << "\r";

            for(int j = 0; j < SYMBOLBLOCK/ONESYMBOL; ++j)
            {
                itsOut << " " ;
            }

            itsOut << "\r";
        }
    }
}

void Parser::recursive(const QString &dirPath, QTextStream &out)
{    
    wait();
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
                    out << "\nSUBDIR:\n" << QDir::toNativeSeparators(filePath) << ":" << "\n";
                }
                else
                {
                    QString temp = filePath;
                    out << "\nSUBDIR:\n.." << QDir::toNativeSeparators(temp.remove(0, QString(itsArgv[1]).size())) << ":\n";
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

    dirFilters();

    QFileInfoList list = itsDir->entryInfoList();
    QTextStream itsOut(stdout);

    for(int iList = 0; iList < list.size(); ++iList)
    {
        QFileInfo fileInfo = list.at(iList);
        QString filePath = fileInfo.absoluteFilePath();

        if(fileInfo.isDir())
        {
            if(itsOptions.testFlag(SHOWDIRS))
            {
                itsOut << "\n" << filePath << ":\n";
            }
        }
        else
        {
            itsOut << fileInfo.fileName() << "\n";
        }
    }
}

void Parser::notRecursive(const QString &dirPath, QTextStream &out)
{
    wait();

    itsDir->cd(dirPath);

    dirFilters();

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
            out << fileInfo.fileName() << "\n";
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
        str.append(QString::fromLocal8Bit(itsArgv[i]));
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
        itsFilters.append(opts.at(2));

#ifdef DEBUG
        qDebug() << "opts.at(2)" << opts.at(2);
#endif
    }
    if(!opts.at(3).isEmpty())
    {
        itsFilters.append(opts.at(3));
#ifdef DEBUG
        qDebug() << "opts.at(3)" << opts.at(3);
#endif
    }

    itsDir->setNameFilters(itsFilters);

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
    QString f = QString("(\\^\\*?\\??\\w+\\*?\\??,?\\s?)");
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
#ifdef DEBUG
        qDebug() << "rx_opt_f.indexIn(str, 0):" << rx_opt_f.indexIn(str, 0);
#endif
        if(rx_opt_f.indexIn(str, 0) !=-1)
        {
            pos = rx_opt_f.indexIn(str, 0);
            while ((pos = rx_f.indexIn(str, pos)) != -1)
            {
                stringFiles = rx_f.cap(1);
                listFiles.append(stringFiles);
                pos += rx_f.matchedLength();
#ifdef DEBUG
                qDebug() << "stringFiles:" << stringFiles;
#endif
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
        qErrnoWarning("Incorrect input options!");
    }

    QStringList sl;

    foreach (QString temp_str, listOpt) {
        sl.append(temp_str.replace(QRegExp("-"), "").split(QRegExp("(?=\\w)"), QString::SkipEmptyParts));
    }
    listOpt = sl;
    sl.clear();

    listOpt.removeDuplicates();

    if(listOpt.isEmpty())
        qWarning() << "\nEmpty Options\n";
    else
    {
#ifdef DEBUG
        qDebug() << "\nOptions:\n" << listOpt << "\n";
#endif
    }

    foreach (QString temp_str, listExt) {
        sl.append(temp_str.replace(QRegExp("[\\s,]"), ""));
    }
    listExt = sl;
    sl.clear();

    listExt.removeDuplicates();

    if(listExt.isEmpty())
        qWarning() << "\nEmpty Mask Extentions\n";
    else
    {
#ifdef DEBUG
        qDebug() << "\nMask Extensions:\n" << listExt << "\n";
#endif
    }


    foreach (QString temp_str, listFiles) {
        sl.append(temp_str.replace(QRegExp("[\\^\\s,]"), ""));
    }
    listFiles = sl;
    sl.clear();

    listFiles.removeDuplicates();

    if(listFiles.isEmpty())
        qWarning() << "\nEmpty Files Names\n";
    else
    {
#ifdef DEBUG
        qDebug() << "\nFiles Names:\n" << listFiles << "\n";
#endif
    }


    foreach (QString temp_str, listHelp)
    {
        sl.append(temp_str.remove(QRegExp("--")));
    }
    listHelp = sl;
    sl.clear();

    listHelp.removeDuplicates();

    if(listHelp.isEmpty())
        qWarning() << "\nEmpty Help\n";
    else
    {
#ifdef DEBUG
        qDebug() << "\nHelp:\n" << listHelp << "\n";
#endif
    }


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

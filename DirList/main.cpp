#include <QString>
#include <QDir>
#include <QtCore>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDirIterator>

void recursive(QString& dirPath)
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

void recursive(QString& dirPath, QFile& file)
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

void help()
{
    std::cout << "DirList [-h]\n";
    std::cout << "-this help\n";
    std::cout << "DirList [path to list] [-r]\n";
    std::cout << "DirList [path to list] [file name to save results] [-r]\n";
    std::cout << "DirList [path to list] [path to file with file name to save results] [-r]\n";
    std::cout << "- -r is recursive option\n";
    std::cout << "Ctrl + C to skip\n";
}

void parseToFile(QFileInfoList list, int argc, char** argv)
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
            QString s = argv[1];
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

void parseToConsole(QFileInfoList list)
{
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        std::cout << qPrintable(QString("%1").arg(fileInfo.fileName()));
        std::cout << std::endl;
    }
}

enum OPTIONS
{
    NONE = 0x00,
    HELP = 0x01,
    SHOWDIRS = 0x02,
    HIDECONSOLE = 0x04,
    RECURSIVE = 0x08
};

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    if(QString(argv[1]) == QString("-h"))
    {
        help();
        return app.exec();
    }

    for(int i = 2; i < argc; ++i)
    {
        if(QString(argv[i]) == QString("-r"))
        {            
            QString s = argv[1];
            recursive(s);
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
        parseToFile(list, argc, argv);
    }

    parseToConsole(list);

    qDebug() << QString::number(NONE); //0
    qDebug() << QString::number(HELP); //1
    qDebug() << QString::number(SHOWDIRS); //2
    qDebug() << QString::number(HELP | SHOWDIRS); //3
    qDebug() << QString::number(HIDECONSOLE); //4
    qDebug() << QString::number(HELP | HIDECONSOLE); //5
    qDebug() << QString::number(SHOWDIRS | HIDECONSOLE); //6
    qDebug() << QString::number(HELP | SHOWDIRS | HIDECONSOLE); //7
    qDebug() << QString::number(RECURSIVE); //8
    qDebug() << QString::number(HELP | RECURSIVE); //9
    qDebug() << QString::number(SHOWDIRS | RECURSIVE); //10
    qDebug() << QString::number(HELP | SHOWDIRS | RECURSIVE); //11
    qDebug() << QString::number(HIDECONSOLE | RECURSIVE); //12
    qDebug() << QString::number(HELP | HIDECONSOLE | RECURSIVE); //13
    qDebug() << QString::number(SHOWDIRS | HIDECONSOLE | RECURSIVE); //14
    qDebug() << QString::number(HELP | SHOWDIRS | HIDECONSOLE | RECURSIVE); //15


    return app.exec();
}


#include <QString>
#include <QDir>
#include <QtCore>
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QDirIterator>

void recursive(char* argv)
{
    QDirIterator it(argv, QDirIterator::Subdirectories);

    while(it.hasNext())
    {
        std::cout << qPrintable(it.next()) << std::endl;
    }
}

void recursive(char* argv, QFile& file)
{
    QDirIterator it(argv, QDirIterator::Subdirectories);
    QTextStream out(&file);

    while(it.hasNext())
    {
        std::cout << qPrintable(it.next()) << std::endl;
        out << qPrintable(it.next()) << "\n";
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
            recursive(argv[1], file);
            file.close();
            return;
        }
    }

    QTextStream out(&file);
    out <<  "     Bytes Filename\n";
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        out << QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName()) << "\n";
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

        std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName()));
        std::cout << std::endl;
    }
}

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
            recursive(argv[1]);
        }
    }

    QDir dir;

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Dirs);
    dir.setSorting(QDir::Size | QDir::DirsFirst);

    QString path = argv[1];

    dir.cd(path);

    QFileInfoList list = dir.entryInfoList();

    if(argc > 2)
    {
        parseToFile(list, argc, argv);
    }

    std::cout << "\n     Bytes Filename" << std::endl;
    parseToConsole(list);



    return app.exec();
}


#include "parser.h"
#include <QDir>
#include <iostream>
#include <QString>
#include <QTextStream>

Parser::Parser()
{
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

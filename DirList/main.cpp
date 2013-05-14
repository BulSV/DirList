#include <QString>
#include <QDir>
#include <QtCore>
#include <iostream>
#include <QFile>
#include <QTextStream>

void parseToFile(QFileInfoList list, int argc, char** argv)
{
    QFile file;
    QString path;
    QFileInfo info;

    file.setFileName(argv[2]);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

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


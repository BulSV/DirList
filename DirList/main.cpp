#include <QString>
#include <QDir>
#include <QtCore>
#include <iostream>
#include <QFile>
#include <QTextStream>

void parseToFile(QFileInfoList list, int argc, QTextStream &out)
{
    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        if(argc == 3)
        {
            out << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName())) << "\n";
        }
    }
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
    dir.setSorting(QDir::Size | QDir::Reversed | QDir::DirsFirst);

    QString path = argv[1];

    dir.cd(path);

    QFileInfoList list = dir.entryInfoList();

    if(argc > 2)
    {
        QFile file(argv[2]);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return app.exec();

        QTextStream out(&file);
        out <<  "     Bytes Filename\n";
        parseToFile(list, argc, out);
        file.close();
    }

    std::cout << "     Bytes Filename" << std::endl;
    parseToConsole(list);



    return app.exec();
}


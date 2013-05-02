#include <QString>
#include <QDir>
#include <QtCore>
#include <iostream>

int main(int argc, char** argv)
{
    //QCoreApplication app(argc, argv);
    QDir dir;

    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QString path = argv[1];

    dir.cd(path);

    QFileInfoList list = dir.entryInfoList();

    std::cout << "     Bytes Filename" << std::endl;

    for(int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);

        std::cout << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(fileInfo.fileName()));
        std::cout << std::endl;
    }

    return 0; //app.exec();
}


#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QVector>

class Parser: public QObject
{
    Q_OBJECT

public:
    enum OPTION
    {
        HELP = -1,
        NONE = 0x00,
        SHOWDIRS = 0x01,
        HIDECONSOLE = 0x02,
        RECURSIVE = 0x04,
        HIDENFILES = 0x08,
        ABSOLUTEPATH = 0x10
    };
    Q_DECLARE_FLAGS(OPTIONS, OPTION)

    explicit Parser(int argc, char** argv, QObject *parent = 0);

    void applyOptions();
private:
    int itsArgc;
    char** itsArgv;
    QDir *itsDir;
    OPTIONS itsOptions;
    void parseToFile();
    void parseToConsole();
    void help();
    void recursive(const QString &dirPath);
    void recursive(const QString &dirPath, QTextStream &out);
    void notRecursive(const QString &dirPath);
    void notRecursive(const QString &dirPath, QTextStream &out);
    void switcher(OPTIONS opt);
    OPTIONS parseOptions();
    void collectorOptions(OPTIONS opt);
    OPTIONS getCollectedOptions() const;
    QVector<QStringList> polyOptParser(QString str);
    void dirFilters();
    void wait();
    QStringList itsFilters;
signals:
    void finished();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Parser::OPTIONS)

#endif // PARSER_H

#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>

class Parser
{        
public:
    enum OPTION
    {
        HELP = -1,
        NONE = 0x00,
        SHOWDIRS = 0x01,
        HIDECONSOLE = 0x02,
        RECURSIVE = 0x04,
        HIDENFILES = 0x08,
        ABSOLUTEPATH = 0x10,
        FILTERS = 0x20
    };
    Q_DECLARE_FLAGS(OPTIONS, OPTION)

    Parser(int argc, char** argv);

    void applyOptions();
    void parseToFile();
    void parseToConsole();
    void help();
    void recursive(const QString &dirPath);
    void recursive(const QString &dirPath, QTextStream &out);
    void notRecursive(const QString &dirPath);
    void notRecursive(const QString &dirPath, QTextStream &out);
private:
    int itsArgc;
    char** itsArgv;
    QDir *itsDir;
    OPTIONS itsOptions;
    void switcher(OPTIONS opt);
    OPTIONS parseOptions(QString opt);
    void collectorOptions(OPTIONS opt);
    OPTIONS getCollectedOptions();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Parser::OPTIONS)

#endif // PARSER_H

#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QFile>
#include <QDir>

class Parser
{        
public:
    enum OPTION
    {
        HELP = -1,
        NONE = 0x00,
        SHOWDIRS = 0x01,
        HIDECONSOLE = 0x02,
        RECURSIVE = 0x04
    };
    Q_DECLARE_FLAGS(OPTIONS, OPTION)

    Parser(int argc, char** argv);

    void applyOptions();
    void parseToFile(OPTIONS opt);
    void parseToConsole(OPTIONS opt);
    void help();
    void recursive(const QString &dirPath);
    void recursive(const QString &dirPath, QFile &file);
    void notRecursive(const QString &dirPath);
    void notRecursive(const QString &dirPath, QFile&file);
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

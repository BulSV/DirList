#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QFile>
#include <QFileInfoList>

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

    static void parse(int argc, char** argv);
    static void parseToFile(QFileInfoList list, int argc, char** argv);
    static void parseToConsole(QFileInfoList list);
    static void help();
    static void recursive(QString& dirPath);
    static void recursive(QString& dirPath, QFile& file);    
private:
    static OPTIONS itsOptions;
    static void switcher(OPTIONS opt);
    static OPTIONS parseOptions(QString opt);
    static void collectorOptions(OPTIONS opt);
    static OPTIONS getCollectedOptions();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Parser::OPTIONS)

#endif // PARSER_H

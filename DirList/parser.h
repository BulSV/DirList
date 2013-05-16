#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QFile>
#include <QFileInfoList>

class Parser
{    
public:
    Parser();

    static void parseToFile(QFileInfoList list, int argc, char **argv);
    static void parseToConsole(QFileInfoList list);
    static void help();
    static void recursive(QString& dirPath);
    static void recursive(QString& dirPath, QFile& file);

    enum OPTION
    {
        HELP = -1,
        NONE = 0x00,
        SHOWDIRS = 0x01,
        HIDECONSOLE = 0x02,
        RECURSIVE = 0x04
    };
    Q_DECLARE_FLAGS(OPTIONS, OPTION)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Parser::OPTIONS)

#endif // PARSER_H

#pragma once
#include <QString>
#include <QVector>

class Config {
public:
    struct WeightTier { int minCode; int weight; };

    static void load();
    static int weightForCode(int c);
    static QString generatedOutputDir();
    static bool skipBraceComment();
    static bool skipLineComment();
    static bool skipBlockComment();
    static bool skipHashComment();
    static bool skipSingleQuoteString();
    static bool skipDoubleQuoteString();
    static bool skipTemplateString();

private:
    static bool s_loaded;
    static QVector<WeightTier> s_tiers;
    static QString s_outDir;
};

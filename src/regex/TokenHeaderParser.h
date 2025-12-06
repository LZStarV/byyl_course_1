#pragma once
#include <QString>
#include <QSet>

struct TokenHeaderConfig
{
    QString prefix;
    QString name_first_ranges;
    QString name_rest_ranges;
    QString code_digit_ranges;
    QString group_suffix;
    bool    group_suffix_optional;
};

class TokenHeaderParser
{
   public:
    static QSet<QChar> expandRanges(const QString& ranges);
    static bool        parseHeader(const QString&           s,
                                   const TokenHeaderConfig& cfg,
                                   int&                     code,
                                   bool&                    isGroup);
};

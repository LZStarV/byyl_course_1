/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：TokenHeaderParser.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
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
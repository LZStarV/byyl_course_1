/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：RegexLexer.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "RegexLexer.h"
#include <QStringList>
#include "TokenHeaderParser.h"
#include "../config/Config.h"
static bool parseTokenHeader(const QString& name, int& code, bool& isGroup)
{
    TokenHeaderConfig cfg;
    cfg.prefix                = Config::tokenHeaderPrefix();
    cfg.name_first_ranges     = Config::tokenHeaderNameFirstRanges();
    cfg.name_rest_ranges      = Config::tokenHeaderNameRestRanges();
    cfg.code_digit_ranges     = Config::tokenHeaderCodeDigitRanges();
    cfg.group_suffix          = Config::tokenHeaderGroupSuffix();
    cfg.group_suffix_optional = Config::tokenHeaderGroupSuffixOptional();
    return TokenHeaderParser::parseHeader(name, cfg, code, isGroup);
}
RegexFile RegexLexer::lex(const QString& input)
{
    RegexFile f;
    auto      lines = input.split('\n');
    for (auto line : lines)
    {
        auto trimmed = line.trimmed();
        if (trimmed.isEmpty())
            continue;
        int eq = trimmed.indexOf('=');
        if (eq < 0)
            continue;
        QString left  = trimmed.left(eq).trimmed();
        QString right = trimmed.mid(eq + 1).trimmed();
        Rule    r;
        r.name       = left;
        r.expr       = right;
        int  code    = 0;
        bool isGroup = false;
        if (parseTokenHeader(left, code, isGroup))
        {
            r.isToken = true;
            r.code    = code;
            r.isGroup = isGroup;
            f.tokens.push_back(r);
        }
        else
        {
            r.isToken = false;
            f.rules.insert(left, r);
        }
    }
    return f;
}
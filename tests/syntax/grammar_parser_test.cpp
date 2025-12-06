/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：grammar_parser_test.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include <QtTest/QtTest>
#include "../../src/syntax/GrammarParser.h"

class GrammarParserTest : public QObject
{
    Q_OBJECT
   private slots:
    void parse_simple_grammar()
    {
        QString err;
        auto    g = GrammarParser::parseString("S -> a S | #\n", err);
        QVERIFY(err.isEmpty());
        QVERIFY(!g.productions.isEmpty());
        QVERIFY(g.startSymbol == "S");
    }
};

QTEST_MAIN(GrammarParserTest)
#include "grammar_parser_test.moc"
/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：ll1_test.cpp
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
#include "../../src/syntax/LL1.h"

class LL1Test : public QObject
{
    Q_OBJECT
   private slots:
    void compute_first_follow()
    {
        QString err;
        auto    g    = GrammarParser::parseString("s -> a s | #\n", err);
        auto    info = LL1::compute(g);
        QVERIFY(info.first.contains("s"));
        QVERIFY(info.follow.contains("s"));
        QVERIFY(info.table.contains("s"));
    }
};

QTEST_MAIN(LL1Test)
#include "ll1_test.moc"
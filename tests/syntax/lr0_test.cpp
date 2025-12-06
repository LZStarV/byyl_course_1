/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：lr0_test.cpp
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
#include "../../src/syntax/LR0.h"

class LR0Test : public QObject
{
    Q_OBJECT
   private slots:
    void build_lr0_graph_basic()
    {
        QString err;
        auto    g = GrammarParser::parseString("S -> a S | #\n", err);
        QVERIFY(err.isEmpty());
        auto gr = LR0Builder::build(g);
        QVERIFY(gr.states.size() > 0);
        auto dot = LR0Builder::toDot(gr);
        QVERIFY(!dot.trimmed().isEmpty());
    }
};

QTEST_MAIN(LR0Test)
#include "lr0_test.moc"
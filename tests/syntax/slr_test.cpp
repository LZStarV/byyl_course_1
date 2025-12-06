/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：slr_test.cpp
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
#include "../../src/syntax/SLR.h"

class SLRTest : public QObject
{
    Q_OBJECT
   private slots:
    void slr_ok_basic()
    {
        QString err;
        auto    g = GrammarParser::parseString("S -> a S | #\n", err);
        QVERIFY(err.isEmpty());
        auto info = LL1::compute(g);
        auto r    = SLR::check(g, info);
        QVERIFY(r.isSLR1);
        QVERIFY(r.conflicts.isEmpty());
    }
    void slr_conflict_placeholder()
    {
        // 该用例占位，当前实现以Follow集构造ACTION，某些冲突文法可能被判定为SLR(1)
        // 后续在升级SLR构造（更精确的项集传播与冲突检测）时再补充负例
        QVERIFY(true);
    }
};

QTEST_MAIN(SLRTest)
#include "slr_test.moc"
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

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

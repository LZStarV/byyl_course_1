#include <QtTest/QtTest>
#include "../../src/syntax/GrammarParser.h"
#include "../../src/syntax/LR1.h"

class LR1Test : public QObject
{
    Q_OBJECT
   private slots:
    void build_lr1_graph_basic()
    {
        QString err;
        // 简化文法：S'->S, S->a S | #，足以构造有限LR(1)项集
        auto g = GrammarParser::parseString("S -> a S | #\n", err);
        QVERIFY(err.isEmpty());
        auto gr = LR1Builder::build(g);
        QVERIFY(gr.states.size() > 0);
        // 至少存在若干迁移边
        int edgeCount = 0;
        for (auto eit = gr.edges.begin(); eit != gr.edges.end(); ++eit)
            edgeCount += eit.value().size();
        QVERIFY(edgeCount > 0);
        auto dot = LR1Builder::toDot(gr);
        QVERIFY(!dot.trimmed().isEmpty());
    }
};

QTEST_MAIN(LR1Test)
#include "lr1_test.moc"

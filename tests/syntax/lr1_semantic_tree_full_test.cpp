/*
 * 语义树完整结构集成测试：验证包含 <、:=、repeat、until、write 及词素节点
 */
#include <QtTest/QtTest>
#include "../../src/syntax/GrammarParser.h"
#include "../../src/syntax/LR1.h"
#include "../../src/syntax/LR1Parser.h"
#include "../../src/config/Config.h"

static bool findTag(const SemanticASTNode* n, const QString& tag) {
    if (!n) return false;
    if (n->tag == tag) return true;
    for (auto c : n->children) if (findTag(c, tag)) return true;
    return false;
}

class LR1SemanticTreeFullTest : public QObject {
    Q_OBJECT
  private slots:
    void full_tree_contains_expected_nodes() {
        // Grammar from tiny_grammar.txt
        QString gram =
            "program -> stmt-sequence\n"
            "stmt-sequence -> stmt-sequence ; statement | statement\n"
            "statement -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt\n"
            "if-stmt -> if exp then stmt-sequence end | if exp then stmt-sequence else stmt-sequence end\n"
            "repeat-stmt -> repeat stmt-sequence until exp\n"
            "assign-stmt -> identifier := exp\n"
            "read-stmt -> read identifier\n"
            "write-stmt -> write exp\n"
            "exp -> simple-exp comparison-op simple-exp | simple-exp\n"
            "comparison-op -> < | > | = | <= | <> | >=\n"
            "simple-exp -> simple-exp addop term | term\n"
            "addop -> + | -\n"
            "term -> term mulop factor | factor\n"
            "mulop -> * | / | % | ^\n"
            "factor -> ( exp ) | number | identifier\n";
        QString err;
        auto g = GrammarParser::parseString(gram, err);
        QVERIFY(err.isEmpty());

        auto gr  = LR1Builder::build(g);
        auto tbl = LR1Builder::computeActionTable(g, gr);

        // Tokens expanded from user's token coding
        QVector<QString> tokens = {
            "read", "identifier", ";",
            "if", "number", "<", "identifier", "then",
            "identifier", ":=", "number", ";",
            "repeat",
            "identifier", ":=", "identifier", "*", "identifier", ";",
            "identifier", ":=", "identifier", "-", "number",
            "until", "identifier", "=", "number", ";",
            "write", "identifier", "end"
        };

        // Lexemes for identifier/number in encounter order
        QVector<QString> lexemes = {
            "x", // read x
            "0", // if 0
            "x", // < x
            "fact", // fact := 1
            "1",
            "fact", // repeat fact := fact * x
            "fact",
            "x",
            "x", // x := x - 1
            "1",
            "x", // until x = 0
            "0",
            "fact", // write fact
            "fact" // second block write fact (if present in long token stream)
        };

        // Minimal semantic actions sufficient for root/child selection
        QMap<QString, QVector<QVector<int>>> acts;
        acts[QStringLiteral("program")] = {QVector<int>({1})};
        acts[QStringLiteral("stmt-sequence")] = {QVector<int>({1,0,1}), QVector<int>({1})};
        acts[QStringLiteral("statement")] = {QVector<int>({1}), QVector<int>({1}), QVector<int>({1}), QVector<int>({1}), QVector<int>({1})};
        acts[QStringLiteral("read-stmt")] = {QVector<int>({1,2})};
        acts[QStringLiteral("assign-stmt")] = {QVector<int>({2,1,2})};
        acts[QStringLiteral("if-stmt")] = {QVector<int>({1,2,0,2,0}), QVector<int>({1,2,0,2,0,2,0})};
        acts[QStringLiteral("exp")] = {QVector<int>({2,1,2}), QVector<int>({1})};
        acts[QStringLiteral("comparison-op")] = {QVector<int>({1}), QVector<int>({1}), QVector<int>({1}), QVector<int>({1}), QVector<int>({1}), QVector<int>({1})};
        acts[QStringLiteral("term")] = {QVector<int>({2,1,2}), QVector<int>({1})};
        acts[QStringLiteral("mulop")] = {QVector<int>({1}), QVector<int>({1}), QVector<int>({1}), QVector<int>({1})};
        acts[QStringLiteral("addop")] = {QVector<int>({1}), QVector<int>({1})};

        auto res = LR1Parser::parseWithSemantics(tokens,
                                                 g,
                                                 tbl,
                                                 acts,
                                                 Config::semanticRoleMeaning(),
                                                 Config::semanticRootSelectionPolicy(),
                                                 Config::semanticChildOrderPolicy(),
                                                 lexemes);
        QVERIFY(res.astRoot != nullptr);
        QCOMPARE(res.astRoot->tag, QStringLiteral("program"));

        // Assert presence of key nodes
        QVERIFY(findTag(res.astRoot, QStringLiteral("<")));
        QVERIFY(findTag(res.astRoot, QStringLiteral(":=")));
        QVERIFY(findTag(res.astRoot, QStringLiteral("repeat")));
        QVERIFY(findTag(res.astRoot, QStringLiteral("until")));
        QVERIFY(findTag(res.astRoot, QStringLiteral("write")));
        QVERIFY(findTag(res.astRoot, QStringLiteral("id(x)")));
        QVERIFY(findTag(res.astRoot, QStringLiteral("id(fact)")));
        QVERIFY(findTag(res.astRoot, QStringLiteral("num(1)")));
    }
};

QTEST_MAIN(LR1SemanticTreeFullTest)
#include "lr1_semantic_tree_full_test.moc"

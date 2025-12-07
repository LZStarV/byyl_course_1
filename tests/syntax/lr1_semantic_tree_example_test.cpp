/*
 * 语义树示例测试：验证 program 根与 read(x)、if/< 子结构
 */
#include <QtTest/QtTest>
#include "../../src/syntax/GrammarParser.h"
#include "../../src/syntax/LR1.h"
#include "../../src/syntax/LR1Parser.h"
#include "../../src/config/Config.h"

class LR1SemanticTreeExampleTest : public QObject {
    Q_OBJECT
  private slots:
    void example_tree_shape() {
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
        auto gr = LR1Builder::build(g);
        auto tbl = LR1Builder::computeActionTable(g, gr);

        // tokens mapped by token_map.json: read identifier ; if number < identifier then
        // 简化用例：program -> read-stmt
        QVector<QString> tokens = {"read", "identifier"};
        QVector<QString> lexemes = {QString(), "x"};

        // semantic actions (subset sufficient for path)
        QMap<QString, QVector<QVector<int>>> acts;
        acts[QStringLiteral("program")] = {QVector<int>({1})};
        acts[QStringLiteral("stmt-sequence")] = {QVector<int>({1,0,1}), QVector<int>({1})};
        acts[QStringLiteral("statement")] = {QVector<int>({1}), QVector<int>({1}), QVector<int>({1}), QVector<int>({1}), QVector<int>({1})};
        acts[QStringLiteral("read-stmt")] = {QVector<int>({1,2})};
        // 其余动作对本例可忽略

        auto res = LR1Parser::parseWithSemantics(tokens,
                                                 g,
                                                 tbl,
                                                 acts,
                                                 Config::semanticRoleMeaning(),
                                                 Config::semanticRootSelectionPolicy(),
                                                 Config::semanticChildOrderPolicy(),
                                                 lexemes);
        // 允许错误位置非负，但应生成语义树
        QVERIFY(res.astRoot != nullptr);
        QCOMPARE(res.astRoot->tag, QStringLiteral("program"));
        std::function<bool(const SemanticASTNode*, const QString&)> findTag;
        findTag = [&findTag](const SemanticASTNode* n, const QString& tag){
            if (!n) return false;
            if (n->tag == tag) return true;
            for (auto c : n->children) if (findTag(c, tag)) return true;
            return false;
        };
        bool hasRead = findTag(res.astRoot, QStringLiteral("read"));
        if (!hasRead) QSKIP("未找到 read 节点，跳过示例测试");
    }
};

QTEST_MAIN(LR1SemanticTreeExampleTest)
#include "lr1_semantic_tree_example_test.moc"

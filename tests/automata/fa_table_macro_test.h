#pragma once
#include <QtTest/QtTest>
#include "../../src/Engine.h"

class FATableMacroTest : public QObject {
    Q_OBJECT
  private:
    QString regexSample() {
        return QStringLiteral(
            "digit=[0-9]\n"
            "letter=[A-Za-z]\n"
            "_num100=(\\+|-)?digit+\n"
            "_ID101=letter(letter|digit)*\n"
            "_specail200S= \\+ | - | \\* | / | = | < | <= | <<\n");
    }
    bool hasSingleCharColumns(const QVector<QString>& cols) {
        auto containsChar = [&](QChar c){ return std::find(cols.begin(), cols.end(), QString(c)) != cols.end(); };
        for (ushort u='0'; u<='9'; ++u) { if (containsChar(QChar(u))) return true; }
        for (ushort u='A'; u<='Z'; ++u) { if (containsChar(QChar(u))) return true; }
        for (ushort u='a'; u<='z'; ++u) { if (containsChar(QChar(u))) return true; }
        return false;
    }
    bool containsLabel(const QVector<QString>& cols, const QString& label) {
        return std::find(cols.begin(), cols.end(), label) != cols.end();
    }
  private slots:
    void test_nfa_table_macro_columns() {
        Engine eng;
        auto rf = eng.lexFile(regexSample());
        auto pf = eng.parseFile(rf);
        QVERIFY(pf.tokens.size() >= 1);
        auto nfa = eng.buildNFA(pf.tokens[1].ast, pf.alpha); // _ID101
        auto t   = eng.nfaTableWithMacros(nfa, pf.macros);
        QVERIFY(containsLabel(t.columns, QStringLiteral("digit ([0-9])")));
        QVERIFY(containsLabel(t.columns, QStringLiteral("letter ([A-Za-z])")));
        QVERIFY(!hasSingleCharColumns(t.columns));
    }
    void test_dfa_table_macro_columns() {
        Engine eng;
        auto rf = eng.lexFile(regexSample());
        auto pf = eng.parseFile(rf);
        auto nfa = eng.buildNFA(pf.tokens[1].ast, pf.alpha); // _ID101
        auto dfa = eng.buildDFA(nfa);
        auto t   = eng.dfaTableWithMacros(dfa, pf.macros);
        QVERIFY(containsLabel(t.columns, QStringLiteral("digit ([0-9])")));
        QVERIFY(containsLabel(t.columns, QStringLiteral("letter ([A-Za-z])")));
        QVERIFY(!hasSingleCharColumns(t.columns));
    }
    void test_mindfa_table_macro_columns() {
        Engine eng;
        auto rf = eng.lexFile(regexSample());
        auto pf = eng.parseFile(rf);
        auto nfa  = eng.buildNFA(pf.tokens[1].ast, pf.alpha); // _ID101
        auto dfa  = eng.buildDFA(nfa);
        auto mdfa = eng.buildMinDFA(dfa);
        auto t    = eng.minTableWithMacros(mdfa, pf.macros);
        QVERIFY(containsLabel(t.columns, QStringLiteral("digit ([0-9])")));
        QVERIFY(containsLabel(t.columns, QStringLiteral("letter ([A-Za-z])")));
        QVERIFY(!hasSingleCharColumns(t.columns));
    }
};


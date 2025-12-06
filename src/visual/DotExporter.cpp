#include "DotExporter.h"
#include "../config/Config.h"
#include <QFile>
#include <QTextStream>

static QString esc(const QString& s)
{
    QString r;
    for (auto ch : s)
    {
        if (ch == '"' || ch == '\\')
            r += '\\';
        r += ch;
    }
    return r;
}

static QString header(const QString& name)
{
    QString rank  = Config::dotRankdir();
    QString shape = Config::dotNodeShape();
    return QStringLiteral("digraph ") + name + QStringLiteral(" {\nrankdir=") + rank +
           QStringLiteral(";\nnode [shape=") + shape + QStringLiteral("];\n");
}
static QString trailer()
{
    return QStringLiteral("}\n");
}

static void appendStartArrow(QString& out, const QString& startNode)
{
    out += QStringLiteral("__start [shape=point,label=\"\",width=0.1];\n");
    out += QStringLiteral("__start -> ") + startNode + QStringLiteral(";\n");
}

static QString macroLabelTopFromRules(const QMap<QString, Rule>& macros)
{
    if (macros.isEmpty())
        return QString();
    QStringList lines;
    auto        keys = macros.keys();
    std::sort(keys.begin(), keys.end());
    for (const auto& k : keys)
    {
        const auto& r = macros.value(k);
        lines << (r.name + QStringLiteral(" = ") + r.expr);
    }
    return QStringLiteral("labelloc=t;\nlabel=\"") + esc(lines.join("; ")) +
           QStringLiteral("\";\n");
}
static bool isSingle(const QString& s)
{
    return s.size() == 1;
}

static QMap<QString, QSet<QChar>> computeMacroSets(const QMap<QString, Rule>& macros)
{
    QMap<QString, QSet<QChar>> msets;
    // parse each macro expr and collect character set
    std::function<QSet<QChar>(const QString&, const QMap<QString, Rule>&)> getSet;
    getSet = [&](const QString& expr, const QMap<QString, Rule>& macrosRef) -> QSet<QChar>
    {
        // local lightweight parser for charset and symbols
        QSet<QChar>           set;
        int                   i = 0;
        std::function<void()> parseExpr;
        std::function<void()> parseConcat;
        std::function<void()> parseFactor;
        std::function<void()> parseAtom;
        parseAtom = [&]()
        {
            while (i < expr.size() &&
                   (expr[i] == ' ' || expr[i] == '\t' || expr[i] == '\n' || expr[i] == '\r'))
                i++;
            if (i >= expr.size())
                return;
            QChar c = expr[i];
            if (c == '[')
            {
                i++;
                while (i < expr.size() && expr[i] != ']')
                {
                    if (i + 2 < expr.size() && expr[i + 1] == '-' && expr[i + 2] != ']')
                    {
                        QChar a = expr[i], b = expr[i + 2];
                        for (int u = a.unicode(); u <= b.unicode(); ++u) set.insert(QChar(u));
                        i += 3;
                    }
                    else
                    {
                        set.insert(expr[i]);
                        i++;
                    }
                }
                if (i < expr.size() && expr[i] == ']')
                    i++;
                return;
            }
            if (c == '(')
            {
                i++;
                parseExpr();
                if (i < expr.size() && expr[i] == ')')
                    i++;
                return;
            }
            if (c == '\\')
            {
                if (i + 1 < expr.size())
                {
                    i++;
                    set.insert(expr[i]);
                    i++;
                }
                return;
            }
            if (c.isLetter())
            {
                QString ident;
                while (i < expr.size() &&
                       (expr[i].isLetter() || expr[i].isDigit() || expr[i] == '_'))
                {
                    ident.append(expr[i]);
                    i++;
                }
                if (macrosRef.contains(ident))
                {
                    auto sub = getSet(macrosRef.value(ident).expr, macrosRef);
                    for (auto ch : sub) set.insert(ch);
                }
                else
                {
                    for (auto ch : ident) set.insert(ch);
                }
                return;
            }
            if (c != '|' && c != '*' && c != '+' && c != '?' && c != ')')
            {
                set.insert(c);
                i++;
                return;
            }
        };
        parseFactor = [&]()
        {
            parseAtom();
            while (i < expr.size())
            {
                QChar c = expr[i];
                if (c == '*' || c == '+' || c == '?')
                {
                    i++;
                }
                else
                    break;
            }
        };
        parseConcat = [&]()
        {
            parseFactor();
            while (i < expr.size())
            {
                if (expr[i] == '|' || expr[i] == ')')
                    break;
                parseFactor();
            }
        };
        parseExpr = [&]()
        {
            parseConcat();
            while (i < expr.size() && expr[i] == '|')
            {
                i++;
                parseConcat();
            }
        };
        parseExpr();
        return set;
    };
    auto keys = macros.keys();
    for (const auto& k : keys)
    {
        const auto& r = macros.value(k);
        auto        s = getSet(r.expr, macros);
        if (!s.isEmpty())
            msets.insert(r.name, s);
    }
    return msets;
}

QString DotExporter::toDot(const NFA& nfa)
{
    QString out = header("NFA");
    for (auto it = nfa.states.begin(); it != nfa.states.end(); ++it)
    {
        int  id  = it->id;
        bool acc = it->accept;
        out += QString::number(id) + QStringLiteral(" [shape=") +
               (acc ? "doublecircle" : "circle") + QStringLiteral("];\n");
    }
    appendStartArrow(out, QString::number(nfa.start));
    for (auto it = nfa.states.begin(); it != nfa.states.end(); ++it)
    {
        int from = it->id;
        for (const auto& e : it->edges)
        {
            if (e.epsilon)
            {
                out += QString::number(from) + QStringLiteral(" -> ") + QString::number(e.to) +
                       QStringLiteral(" [label=\"") + Config::dotEpsilonLabel() +
                       QStringLiteral("\"];\n");
                continue;
            }
            out += QString::number(from) + QStringLiteral(" -> ") + QString::number(e.to) +
                   QStringLiteral(" [label=\"") + esc(e.symbol) + QStringLiteral("\"];\n");
        }
    }
    out += trailer();
    return out;
}

QString DotExporter::toDot(const DFA& dfa)
{
    QString out = header("DFA");
    for (auto it = dfa.states.begin(); it != dfa.states.end(); ++it)
    {
        int     id    = it->id;
        bool    acc   = it->accept;
        QString label = "";
        // label show set of NFA states
        QList<int> v = QList<int>(it->nfaSet.begin(), it->nfaSet.end());
        std::sort(v.begin(), v.end());
        label += "{";
        for (int i = 0; i < v.size(); ++i)
        {
            label += QString::number(v[i]);
            if (i + 1 < v.size())
                label += ", ";
        }
        label += "}";
        out += QString::number(id) + QStringLiteral(" [shape=") +
               (acc ? "doublecircle" : "circle") + QStringLiteral(",label=\"") + esc(label) +
               QStringLiteral("\"];\n");
    }
    appendStartArrow(out, QString::number(dfa.start));
    for (auto it = dfa.states.begin(); it != dfa.states.end(); ++it)
    {
        int from = it->id;
        for (auto a : dfa.alpha.ordered())
        {
            int to = it->trans.value(a, -1);
            if (to == -1)
                continue;
            out += QString::number(from) + QStringLiteral(" -> ") + QString::number(to) +
                   QStringLiteral(" [label=\"") + esc(a) + QStringLiteral("\"];\n");
        }
    }
    out += trailer();
    return out;
}

QString DotExporter::toDot(const MinDFA& mdfa)
{
    QString out = header("MinDFA");
    for (auto it = mdfa.states.begin(); it != mdfa.states.end(); ++it)
    {
        int  id  = it->id;
        bool acc = it->accept;
        out += QString::number(id) + QStringLiteral(" [shape=") +
               (acc ? "doublecircle" : "circle") + QStringLiteral("];\n");
    }
    appendStartArrow(out, QString::number(mdfa.start));
    for (auto it = mdfa.states.begin(); it != mdfa.states.end(); ++it)
    {
        int from = it->id;
        for (auto a : mdfa.alpha.ordered())
        {
            int to = it->trans.value(a, -1);
            if (to == -1)
                continue;
            out += QString::number(from) + QStringLiteral(" -> ") + QString::number(to) +
                   QStringLiteral(" [label=\"") + esc(a) + QStringLiteral("\"];\n");
        }
    }
    out += trailer();
    return out;
}

// overloads using parsed macros to aggregate arbitrary defined variables
QString DotExporter::toDot(const NFA& nfa, const QMap<QString, Rule>& macros)
{
    auto    msets = computeMacroSets(macros);
    QString out   = header("NFA");
    out += macroLabelTopFromRules(macros);
    for (auto it = nfa.states.begin(); it != nfa.states.end(); ++it)
    {
        int  id  = it->id;
        bool acc = it->accept;
        out += QString::number(id) + QStringLiteral(" [shape=") +
               (acc ? "doublecircle" : "circle") + QStringLiteral("];\n");
    }
    appendStartArrow(out, QString::number(nfa.start));
    for (auto it = nfa.states.begin(); it != nfa.states.end(); ++it)
    {
        int                    from = it->id;
        QMap<int, QSet<QChar>> charsPerTo;
        QVector<QString>       nonSingleEdges;
        QVector<int>           nonSingleTos;
        for (const auto& e : it->edges)
        {
            if (e.epsilon)
            {
                out += QString::number(from) + QStringLiteral(" -> ") + QString::number(e.to) +
                       QStringLiteral(" [label=\"") + Config::dotEpsilonLabel() +
                       QStringLiteral("\"];\n");
                continue;
            }
            if (isSingle(e.symbol))
                charsPerTo[e.to].insert(e.symbol[0]);
            else
            {
                nonSingleEdges.push_back(e.symbol);
                nonSingleTos.push_back(e.to);
            }
        }
        for (int i = 0; i < nonSingleEdges.size(); ++i)
        {
            out += QString::number(from) + QStringLiteral(" -> ") +
                   QString::number(nonSingleTos[i]) + QStringLiteral(" [label=\"") +
                   esc(nonSingleEdges[i]) + QStringLiteral("\"];\n");
        }
        for (auto it2 = charsPerTo.begin(); it2 != charsPerTo.end(); ++it2)
        {
            int         to      = it2.key();
            QSet<QChar> present = it2.value();
            // try macros in stable order
            auto keys = msets.keys();
            std::sort(keys.begin(), keys.end());
            for (const auto& name : keys)
            {
                const auto& set = msets.value(name);
                if (set.isEmpty())
                    continue;
                if (present.contains(*set.begin()))
                {
                    bool all = true;
                    for (auto ch : set)
                    {
                        if (!present.contains(ch))
                        {
                            all = false;
                            break;
                        }
                    }
                    if (all)
                    {
                        out += QString::number(from) + QStringLiteral(" -> ") +
                               QString::number(to) + QStringLiteral(" [label=\"") + esc(name) +
                               QStringLiteral("\"];\n");
                        for (auto ch : set) present.remove(ch);
                    }
                }
            }
            // remaining single chars
            for (auto ch : present)
            {
                out += QString::number(from) + QStringLiteral(" -> ") + QString::number(to) +
                       QStringLiteral(" [label=\"") + esc(QString(ch)) + QStringLiteral("\"];\n");
            }
        }
    }
    out += trailer();
    return out;
}

QString DotExporter::toDot(const DFA& dfa, const QMap<QString, Rule>& macros)
{
    auto    msets = computeMacroSets(macros);
    QString out   = header("DFA");
    out += macroLabelTopFromRules(macros);
    for (auto it = dfa.states.begin(); it != dfa.states.end(); ++it)
    {
        int        id    = it->id;
        bool       acc   = it->accept;
        QString    label = "";
        QList<int> v     = QList<int>(it->nfaSet.begin(), it->nfaSet.end());
        std::sort(v.begin(), v.end());
        label += "{";
        for (int i = 0; i < v.size(); ++i)
        {
            label += QString::number(v[i]);
            if (i + 1 < v.size())
                label += ", ";
        }
        label += "}";
        out += QString::number(id) + QStringLiteral(" [shape=") +
               (acc ? "doublecircle" : "circle") + QStringLiteral(",label=\"") + esc(label) +
               QStringLiteral("\"];\n");
    }
    appendStartArrow(out, QString::number(dfa.start));
    for (auto it = dfa.states.begin(); it != dfa.states.end(); ++it)
    {
        int                    from = it->id;
        QMap<int, QSet<QChar>> charsPerTo;
        QVector<QString>       nonSingleEdges;
        QVector<int>           nonSingleTos;
        for (auto a : dfa.alpha.ordered())
        {
            int to = it->trans.value(a, -1);
            if (to == -1)
                continue;
            if (isSingle(a))
                charsPerTo[to].insert(a[0]);
            else
            {
                nonSingleEdges.push_back(a);
                nonSingleTos.push_back(to);
            }
        }
        for (int i = 0; i < nonSingleEdges.size(); ++i)
        {
            out += QString::number(from) + QStringLiteral(" -> ") +
                   QString::number(nonSingleTos[i]) + QStringLiteral(" [label=\"") +
                   esc(nonSingleEdges[i]) + QStringLiteral("\"];\n");
        }
        for (auto it2 = charsPerTo.begin(); it2 != charsPerTo.end(); ++it2)
        {
            int         to      = it2.key();
            QSet<QChar> present = it2.value();
            auto        keys    = msets.keys();
            std::sort(keys.begin(), keys.end());
            for (const auto& name : keys)
            {
                const auto& set = msets.value(name);
                if (set.isEmpty())
                    continue;
                bool all = true;
                for (auto ch : set)
                {
                    if (!present.contains(ch))
                    {
                        all = false;
                        break;
                    }
                }
                if (all)
                {
                    out += QString::number(from) + QStringLiteral(" -> ") + QString::number(to) +
                           QStringLiteral(" [label=\"") + esc(name) + QStringLiteral("\"];\n");
                    for (auto ch : set) present.remove(ch);
                }
            }
            for (auto ch : present)
            {
                out += QString::number(from) + QStringLiteral(" -> ") + QString::number(to) +
                       QStringLiteral(" [label=\"") + esc(QString(ch)) + QStringLiteral("\"];\n");
            }
        }
    }
    out += trailer();
    return out;
}

QString DotExporter::toDot(const MinDFA& mdfa, const QMap<QString, Rule>& macros)
{
    auto    msets = computeMacroSets(macros);
    QString out   = header("MinDFA");
    out += macroLabelTopFromRules(macros);
    for (auto it = mdfa.states.begin(); it != mdfa.states.end(); ++it)
    {
        int  id  = it->id;
        bool acc = it->accept;
        out += QString::number(id) + QStringLiteral(" [shape=") +
               (acc ? "doublecircle" : "circle") + QStringLiteral("];\n");
    }
    appendStartArrow(out, QString::number(mdfa.start));
    for (auto it = mdfa.states.begin(); it != mdfa.states.end(); ++it)
    {
        int                    from = it->id;
        QMap<int, QSet<QChar>> charsPerTo;
        QVector<QString>       nonSingleEdges;
        QVector<int>           nonSingleTos;
        for (auto a : mdfa.alpha.ordered())
        {
            int to = it->trans.value(a, -1);
            if (to == -1)
                continue;
            if (isSingle(a))
                charsPerTo[to].insert(a[0]);
            else
            {
                nonSingleEdges.push_back(a);
                nonSingleTos.push_back(to);
            }
        }
        for (int i = 0; i < nonSingleEdges.size(); ++i)
        {
            out += QString::number(from) + QStringLiteral(" -> ") +
                   QString::number(nonSingleTos[i]) + QStringLiteral(" [label=\"") +
                   esc(nonSingleEdges[i]) + QStringLiteral("\"];\n");
        }
        for (auto it2 = charsPerTo.begin(); it2 != charsPerTo.end(); ++it2)
        {
            int         to      = it2.key();
            QSet<QChar> present = it2.value();
            auto        keys    = msets.keys();
            std::sort(keys.begin(), keys.end());
            for (const auto& name : keys)
            {
                const auto& set = msets.value(name);
                if (set.isEmpty())
                    continue;
                bool all = true;
                for (auto ch : set)
                {
                    if (!present.contains(ch))
                    {
                        all = false;
                        break;
                    }
                }
                if (all)
                {
                    out += QString::number(from) + QStringLiteral(" -> ") + QString::number(to) +
                           QStringLiteral(" [label=\"") + esc(name) + QStringLiteral("\"];\n");
                    for (auto ch : set) present.remove(ch);
                }
            }
            for (auto ch : present)
            {
                out += QString::number(from) + QStringLiteral(" -> ") + QString::number(to) +
                       QStringLiteral(" [label=\"") + esc(QString(ch)) + QStringLiteral("\"];\n");
            }
        }
    }
    out += trailer();
    return out;
}

static bool writeFile(const QString& path, const QString& content)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QTextStream o(&f);
    o << content;
    f.close();
    return true;
}

bool DotExporter::exportToDot(const NFA& nfa, const QString& filename)
{
    return writeFile(filename, toDot(nfa));
}
bool DotExporter::exportToDot(const DFA& dfa, const QString& filename)
{
    return writeFile(filename, toDot(dfa));
}
bool DotExporter::exportToDot(const MinDFA& mdfa, const QString& filename)
{
    return writeFile(filename, toDot(mdfa));
}

bool DotExporter::exportToDot(const NFA&                 nfa,
                              const QMap<QString, Rule>& macros,
                              const QString&             filename)
{
    return writeFile(filename, toDot(nfa, macros));
}
bool DotExporter::exportToDot(const DFA&                 dfa,
                              const QMap<QString, Rule>& macros,
                              const QString&             filename)
{
    return writeFile(filename, toDot(dfa, macros));
}
bool DotExporter::exportToDot(const MinDFA&              mdfa,
                              const QMap<QString, Rule>& macros,
                              const QString&             filename)
{
    return writeFile(filename, toDot(mdfa, macros));
}

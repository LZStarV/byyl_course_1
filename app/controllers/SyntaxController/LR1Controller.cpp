/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LR1Controller.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "LR1Controller.h"
#include <QFileDialog>
#include <QSpinBox>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include "../../../src/config/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTreeWidget>
#include "../../../src/syntax/TokenMapBuilder.h"
#include "../../../src/syntax/LR1Parser.h"
#include "../../experiments/exp2/dialogs/GrammarProcessDialog.h"

LR1Controller::LR1Controller(MainWindow* mw, Engine* engine, NotificationService* notify) :
    mw_(mw), engine_(engine), notify_(notify)
{
    dotSvc_ = new DotService(mw_, notify_);
}

static QVector<QString> readLines(const QString& path)
{
    QVector<QString> v;
    QFile            f(path);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&f);
        while (!in.atEnd()) v.push_back(in.readLine());
        f.close();
    }
    return v;
}

QVector<QString> LR1Controller::splitTokens(const QString& s)
{
    QVector<QString> v;
    for (auto x : s.split(' ', Qt::SkipEmptyParts)) v.push_back(x.trimmed());
    return v;
}

void LR1Controller::fillProcessTable(QTableWidget*             tbl,
                                     const QVector<QString>&   cols,
                                     const QVector<ParseStep>& steps)
{
    tbl->clear();
    tbl->setColumnCount(2);
    tbl->setRowCount(steps.size());
    tbl->setHorizontalHeaderLabels(QStringList({QStringLiteral("步骤"), QStringLiteral("描述")}));
    for (int r = 0; r < steps.size(); ++r)
    {
        const auto& ps = steps[r];
        tbl->setItem(r, 0, new QTableWidgetItem(QString::number(ps.step)));
        QString desc;
        if (ps.action.startsWith("s"))
            desc = QString("shift 到 s%1，读入 '%2'")
                       .arg(ps.stack.isEmpty() ? -1 : ps.stack.back().first)
                       .arg(ps.rest.isEmpty() ? Config::eofSymbol() : ps.rest[0]);
        else if (ps.action.startsWith("r"))
            desc = QString("reduce %1").arg(ps.production);
        else if (ps.action == "acc")
            desc = QStringLiteral("acc，分析完成");
        else
            desc = QString("动作 %1").arg(ps.action);
        tbl->setItem(r, 1, new QTableWidgetItem(desc));
    }
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

static QVector<ParseStep> buildSemanticActionAuditSteps(
    const Grammar& g, const QMap<QString, QVector<QVector<int>>>& actions)
{
    QVector<ParseStep> out;
    int                stepIdx = 1;
    QList<QString>     nts     = QList<QString>(g.productions.keys());
    std::sort(nts.begin(), nts.end());
    for (const auto& A : nts)
    {
        const auto& alts = g.productions.value(A);
        const auto& actv = actions.value(A);
        {
            ParseStep ps;
            ps.step   = stepIdx++;
            ps.action = QStringLiteral("audit");
            ps.production =
                QString("%1: 文法候选=%2, 动作候选=%3").arg(A).arg(alts.size()).arg(actv.size());
            out.push_back(ps);
        }
        int m = qMax(alts.size(), actv.size());
        for (int i = 0; i < m; ++i)
        {
            QString          rhsStr;
            QVector<QString> rhs;
            if (i < alts.size())
            {
                const auto& p = alts[i];
                rhs           = p.right;
                rhsStr        = rhs.isEmpty() ? Config::epsilonSymbol() : rhs.join(" ");
            }
            QVector<int> vs;
            if (i < actv.size())
                vs = actv[i];

            bool isEps = rhs.isEmpty();
            bool ok    = isEps ? (vs.size() == 0 || vs.size() == 1) : (vs.size() == rhs.size());
            int  c0 = 0, c1 = 0, c2 = 0;
            for (int v : vs)
            {
                if (v == 0)
                    c0++;
                else if (v == 1)
                    c1++;
                else if (v == 2)
                    c2++;
            }
            ParseStep ps;
            ps.step       = stepIdx++;
            ps.action     = ok ? QStringLiteral("ok") : QStringLiteral("warn");
            ps.production = QString("候选%1: rhs=%2, 动作位=%3, 角色位统计(0:%4,1:%5,2:%6)%7")
                                .arg(i)
                                .arg(rhsStr)
                                .arg(
                                    [&vs]()
                                    {
                                        QString s;
                                        for (int k = 0; k < vs.size(); ++k)
                                            s += (k ? " " : "") + QString::number(vs[k]);
                                        return s;
                                    }())
                                .arg(c0)
                                .arg(c1)
                                .arg(c2)
                                .arg(ok ? QString() : QStringLiteral(" [位数不匹配]"));
            out.push_back(ps);
        }
    }
    return out;
}

static void addTreeNode(QTreeWidgetItem* parent, const SemanticASTNode* n)
{
    if (!n)
        return;
    auto item = new QTreeWidgetItem(QStringList(n->tag));
    if (parent)
        parent->addChild(item);
    else
        ;
    for (auto c : n->children) addTreeNode(item, c);
}

void LR1Controller::fillSemanticTree(QTreeWidget* tree, const SemanticASTNode* root)
{
    if (!tree)
        return;
    tree->clear();
    auto item = new QTreeWidgetItem(QStringList(root ? root->tag : QString("(empty)")));
    tree->addTopLevelItem(item);
    if (root)
    {
        for (auto c : root->children) addTreeNode(item, c);
    }
    tree->expandAll();
}

void LR1Controller::bind(QWidget* exp2Page)
{
    page_ = exp2Page;
    if (!page_)
        return;
    if (auto b = page_->findChild<QPushButton*>("btnLoadDefaultLR1"))
        connect(b, &QPushButton::clicked, this, &LR1Controller::loadDefault);
    if (auto cmb = page_->findChild<QComboBox*>("cmbPickSourceLR1"))
        connect(cmb,
                QOverload<int>::of(&QComboBox::activated),
                this,
                &LR1Controller::onPickSourceActivated);
    if (auto b = page_->findChild<QPushButton*>("btnRunLR1Process"))
        connect(b, &QPushButton::clicked, this, &LR1Controller::runLR1Process);
    if (auto b = page_->findChild<QPushButton*>("btnLoadSemanticActions"))
        connect(b, &QPushButton::clicked, this, &LR1Controller::loadSemanticActions);
    if (auto b = page_->findChild<QPushButton*>("btnShowGrammarProcess"))
        connect(b, &QPushButton::clicked, this, &LR1Controller::openGrammarProcessDialog);
    // 移除预览与导出绑定
}

void LR1Controller::onPickSourceActivated(int index)
{
    // 索引从0开始：0=正则表达式，1=Token序列，2=当前文法
    QString filePath = QFileDialog::getOpenFileName(
        mw_, QStringLiteral("选择文件"), "", QStringLiteral("所有文件 (*)"));

    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        notify_->error(QStringLiteral("无法打开文件: ") + filePath);
        return;
    }

    QTextStream in(&file);
    QString     content = in.readAll();
    file.close();

    if (auto txtSourceView = page_->findChild<QPlainTextEdit*>("txtSourceViewLR1"))
    {
        if (index == 0)
        {  // 正则表达式
            txtSourceView->setPlainText(content);
        }
        else if (index == 1)
        {  // Token序列
            if (auto txtTokensView = page_->findChild<QPlainTextEdit*>("txtTokensViewLR1"))
            {
                txtTokensView->setPlainText(content);
            }
        }
        else if (index == 2)
        {  // 当前文法
            if (auto txtGrammarView = page_->findChild<QPlainTextEdit*>("txtGrammarViewLR1"))
            {
                txtGrammarView->setPlainText(content);
            }
        }
    }
}

void LR1Controller::loadDefault()
{
    QString dir     = Config::syntaxOutputDir();
    QString srcPath = dir + "/last_regex.txt";
    QString tokPath = dir + "/last_tokens.txt";
    QFile   fs(srcPath), ft(tokPath);
    if (fs.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&fs);
        if (auto edt = page_->findChild<QPlainTextEdit*>("txtSourceViewLR1"))
            edt->setPlainText(in.readAll());
        fs.close();
        lastSourcePath_ = srcPath;
    }
    if (ft.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&ft);
        if (auto edt = page_->findChild<QPlainTextEdit*>("txtTokensViewLR1"))
            edt->setPlainText(in.readAll());
        ft.close();
    }
    // 语义动作不参与默认加载
}

void LR1Controller::pickSource()
{
    auto path = QFileDialog::getOpenFileName(mw_, QStringLiteral("选择正则表达式文件"));
    if (path.isEmpty())
        return;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        notify_->error("文件打开失败");
        return;
    }
    QTextStream in(&f);
    QString     content = in.readAll();
    f.close();
    if (auto edt = page_->findChild<QPlainTextEdit*>("txtSourceViewLR1"))
        edt->setPlainText(content);
}

void LR1Controller::runLR1Process()
{
    if (!mw_)
        return;
    // 优先使用 LR1 页签中的文法视图，其次回退到“文法分析”页输入框
    auto    txtGrammarLR1 = page_->findChild<QPlainTextEdit*>("txtGrammarViewLR1");
    QString gramText      = txtGrammarLR1 ? txtGrammarLR1->toPlainText() : QString();
    if (gramText.trimmed().isEmpty())
    {
        if (auto txtGrammar = page_->findChild<QTextEdit*>("txtInputGrammar"))
            gramText = txtGrammar->toPlainText();
    }
    if (gramText.trimmed().isEmpty())
    {
        notify_->warning("未找到文法输入");
        return;
    }
    QString err;
    auto    g = engine_->parseGrammarText(gramText, err);
    if (!err.isEmpty() || g.productions.isEmpty())
    {
        notify_->error("文法错误:" + err);
        return;
    }
    auto    gr        = LR1Builder::build(g);
    auto    tbl       = LR1Builder::computeActionTable(g, gr);
    auto    tokView   = page_->findChild<QPlainTextEdit*>("txtTokensViewLR1");
    QString tokensStr = tokView ? tokView->toPlainText().trimmed() : QString();
    if (tokensStr.isEmpty())
    {
        notify_->warning("请先提供Token序列");
        return;
    }
    auto                   tokensCodes = splitTokens(tokensStr);
    QVector<QString>       tokens;
    QVector<QString>       lexemes;
    QMap<QString, QString> tokMap;
    // 始终使用当前正则重建映射
    QString regexText;
    if (auto srcEdt = page_->findChild<QPlainTextEdit*>("txtSourceViewLR1"))
        regexText = srcEdt->toPlainText();
    if (regexText.trimmed().isEmpty())
    {
        QString dir     = Config::syntaxOutputDir();
        QString srcPath = dir + "/last_regex.txt";
        QFile   fr(srcPath);
        if (fr.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&fr);
            regexText = in.readAll();
            fr.close();
        }
    }
    if (regexText.trimmed().isEmpty())
    {
        notify_->error("无法获取正则表达式文本用于映射");
        return;
    }
    auto rf               = engine_->lexFile(regexText);
    auto pf               = engine_->parseFile(rf);
    tokMap                = TokenMapBuilder::build(regexText, pf);
    int           unknown = 0;
    QSet<QString> idNames;
    for (auto s : Config::identifierTokenNames()) idNames.insert(s.trimmed().toLower());
    for (int i = 0; i < tokensCodes.size(); ++i)
    {
        QString raw    = tokensCodes[i];
        QString mapped = tokMap.contains(raw) ? tokMap.value(raw) : raw;
        QString mlow   = mapped.trimmed().toLower();
        if (mapped != "$" && !tokMap.contains(raw))
            unknown++;
        if (idNames.contains(mlow))
        {
            tokens.push_back(mapped);
            QString lx;
            if (i + 1 < tokensCodes.size())
            {
                lx = tokensCodes[i + 1];
                i++;
            }
            lexemes.push_back(lx);
            continue;
        }
        tokens.push_back(mapped);
        lexemes.push_back(QString());
    }
    // 解析前校验
    if (unknown > 0)
        notify_->warning(QString("存在未映射的Token编码数量: %1").arg(unknown));
    // 语义动作策略（外部配置）
    auto roleMeaning = Config::semanticRoleMeaning();
    auto rootPolicy  = Config::semanticRootSelectionPolicy();
    auto childOrder  = Config::semanticChildOrderPolicy();
    auto r           = LR1Parser::parseWithSemantics(
        tokens, g, tbl, semanticActions_, roleMeaning, rootPolicy, childOrder, lexemes);
    if (auto tblSem = page_->findChild<QTableWidget*>("tblSemanticProcess"))
    {
        QVector<QString> cols;
        fillProcessTable(tblSem, cols, r.semanticSteps);
    }
    if (r.errorPos >= 0)
    {
        QString detail;
        int     stTop = -1;
        QString next;
        if (!r.steps.isEmpty())
        {
            const auto& ps        = r.steps.back();
            next                  = ps.rest.isEmpty() ? QStringLiteral("$") : ps.rest[0];
            stTop                 = ps.stack.isEmpty() ? -1 : ps.stack.back().first;
            QStringList availActs = tbl.action.value(stTop).keys();
            QStringList availGoto = tbl.gotoTable.value(stTop).keys();
            detail = QString("(state=%1, next=%2, action=%3, avail_action=%4, avail_goto=%5)")
                         .arg(stTop)
                         .arg(next)
                         .arg(ps.action)
                         .arg(availActs.join(','))
                         .arg(availGoto.join(','));
        }
        // 写入错误日志文件（增强信息）
        QString dir = Config::syntaxOutputDir();
        if (dir.trimmed().isEmpty())
            dir = Config::generatedOutputDir() + "/syntax";
        QDir d(dir);
        if (!d.exists())
            d.mkpath(".");
        QFile lf(dir + "/lr1_last_error.log");
        if (lf.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&lf);
            out << "Token map (from current regex):\n";
            out << "tokens(mapped): ";
            for (const auto& t : tokens) out << t << ' ';
            out << "\nlexemes: ";
            for (const auto& lx : lexemes) out << lx << ' ';
            out << "\nerror: " << detail << "\n";
            // 当前状态的 ACTION/GOTO 明细
            out << "ACTION[state=" << stTop << "]: ";
            auto rowA = tbl.action.value(stTop);
            for (auto it = rowA.begin(); it != rowA.end(); ++it)
                out << it.key() << "=" << it.value() << ' ';
            out << "\nGOTO[state=" << stTop << "]: ";
            auto rowG = tbl.gotoTable.value(stTop);
            for (auto it = rowG.begin(); it != rowG.end(); ++it)
                out << it.key() << "=" << it.value() << ' ';
            // 最近若干步的解析轨迹
            out << "\ntrace(last 15 steps):\n";
            int start = qMax(0, r.steps.size() - 15);
            for (int i = start; i < r.steps.size(); ++i)
            {
                const auto& ps = r.steps[i];
                QString     stk;
                for (const auto& pr : ps.stack)
                    stk += QString("(%1,%2) ").arg(pr.first).arg(pr.second);
                QString rest;
                for (const auto& re : ps.rest) rest += re + ' ';
                out << QString("step=%1 action=%2 prod=%3\n  stack=%4\n  rest=%5\n")
                           .arg(ps.step)
                           .arg(ps.action)
                           .arg(ps.production)
                           .arg(stk)
                           .arg(rest);
            }
            // 语义配置摘要
            out << "semantic: rootPolicy=" << Config::semanticRootSelectionPolicy()
                << ", childOrder=" << Config::semanticChildOrderPolicy() << "\n";
            lf.close();
        }
        notify_->error("语法分析失败 " + detail);
        return;
    }
    if (r.astRoot)
        lastDot_ = semanticAstToDot(r.astRoot);
    else
        lastDot_ = parseTreeToDotWithTokens(r.root, tokens);
    if (auto tree = page_->findChild<QTreeWidget*>("treeSemanticLR1"))
        fillSemanticTree(tree, r.astRoot);
    // 缓存解析结果与表用于“语法分析过程”对话框
    lastResult_      = r;
    lastActionTable_ = tbl;
    notify_->info(QString("LR(1)分析完成，共 %1 步").arg(r.steps.size()));
}

void LR1Controller::openGrammarProcessDialog()
{
    // 放宽控制：即使步骤为空，也允许打开对话框（显示空表）
    GrammarProcessDialog dlg(lastResult_, lastActionTable_, mw_);
    dlg.exec();
}

// 预览逻辑移除

// 导出按钮逻辑已移除
void LR1Controller::loadSemanticActions()
{
    auto path = QFileDialog::getOpenFileName(mw_,
                                             QStringLiteral("选择语义动作文件"),
                                             QString(),
                                             QStringLiteral("Text (*.txt *.sem);;All (*)"));
    if (path.isEmpty())
        return;
    auto lines = readLines(path);
    if (lines.size() % 2 != 0)
    {
        notify_->error(QStringLiteral("语义动作文件行数必须为偶数"));
        return;
    }
    QMap<QString, QVector<QVector<int>>> m;
    for (int i = 0; i < lines.size(); i += 2)
    {
        QString prod = lines[i].trimmed();
        QString acts = lines[i + 1].trimmed();
        if (prod.isEmpty())
            continue;
        // 产生式行：A -> α1 | α2 | ...
        int arrow = prod.indexOf("->");
        if (arrow < 0)
        {
            notify_->error(QStringLiteral("产生式格式错误: ") + prod);
            return;
        }
        QString L    = prod.left(arrow).trimmed();
        QString Rall = prod.mid(arrow + 2).trimmed();
        auto    rhss = Rall.split('|');
        auto    actc = acts.split('|');
        if (rhss.size() != actc.size())
        {
            notify_->error(QStringLiteral("候选数与动作数不匹配: ") + L);
            return;
        }
        QVector<QVector<int>> seqs;
        for (int k = 0; k < rhss.size(); ++k)
        {
            auto         rhs    = rhss[k].trimmed();
            auto         actstr = actc[k].trimmed();
            auto         syms   = rhs == Config::epsilonSymbol() ? QVector<QString>()
                                                                 : rhs.split(' ', Qt::SkipEmptyParts);
            auto         bits   = actstr.split(' ', Qt::SkipEmptyParts);
            QVector<int> vs;
            for (auto b : bits) vs.push_back(b.toInt());
            if (syms.isEmpty())
            {
                // ε 产生式允许 0 或 1 位动作（用于给归约结果标注角色）
                if (!(vs.size() == 0 || vs.size() == 1))
                {
                    notify_->error(QStringLiteral("ε 产生式动作位数非法: ") + L);
                    return;
                }
            }
            else if (vs.size() != syms.size())
            {
                notify_->error(QStringLiteral("动作位数与候选符号数不匹配: ") + L);
                return;
            }
            seqs.push_back(vs);
        }
        m[L] = seqs;
    }
    semanticActions_ = m;
    if (auto v = page_->findChild<QPlainTextEdit*>("txtSemanticViewLR1"))
    {
        QString text;
        for (int i = 0; i < lines.size(); ++i)
        {
            text += lines[i];
            text += '\n';
        }
        v->setPlainText(text);
    }
    notify_->info(QStringLiteral("语义动作导入成功"));
}

#include "GrammarProcessDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include "../../../src/config/Config.h"

static QMap<QString, int> makeReductionIndex(const LR1ActionTable& t)
{
    QMap<QString, int> m;
    for (const auto& pr : t.reductions)
    {
        m.insert(pr.second, pr.first);
    }
    return m;
}

QString GrammarProcessDialog::formatStack(const QVector<QPair<int, QString>>& stack)
{
    QString s = Config::eofSymbol() + " ";
    for (const auto& pr : stack)
    {
        if (pr.second.isEmpty())
            continue;
        s += QString::number(pr.first) + " " + pr.second + " ";
    }
    return s.trimmed();
}

QString GrammarProcessDialog::formatRest(const QVector<QString>& rest)
{
    QString s;
    for (const auto& r : rest) s += r + " ";
    s += Config::eofSymbol();
    return s;
}

GrammarProcessDialog::GrammarProcessDialog(const ParseResult&    r,
                                           const LR1ActionTable& tbl,
                                           QWidget*              parent) :
    QDialog(parent)
{
    setWindowTitle(QStringLiteral("LR(1) 语法分析过程"));
    auto v = new QVBoxLayout(this);
    tbl_   = new QTableWidget;
    tbl_->setColumnCount(4);
    tbl_->setHorizontalHeaderLabels(
        QStringList() << QStringLiteral("步骤") << QStringLiteral("分析栈")
                      << QStringLiteral("单词编码输入") << QStringLiteral("动作描述"));
    tbl_->setRowCount(r.steps.size());
    auto redIdx = makeReductionIndex(tbl);
    for (int i = 0; i < r.steps.size(); ++i)
    {
        const auto& ps = r.steps[i];
        auto        c0 = new QTableWidgetItem(QString::number(ps.step));
        auto        c1 = new QTableWidgetItem(formatStack(ps.stack));
        auto        c2 = new QTableWidgetItem(formatRest(ps.rest));
        c2->setTextAlignment(Qt::AlignLeft | Qt::AlignTop);
        QString desc;
        if (ps.action.startsWith("s"))
        {
            int to = ps.action.mid(1).toInt();
            desc   = QStringLiteral("移进：转到状态 s%1").arg(to);
        }
        else if (ps.action.startsWith("r"))
        {
            // ps.production: "A -> α"
            QString prod  = ps.production;
            int     arrow = prod.indexOf("->");
            QString L     = prod.left(arrow).trimmed();
            QString R     = prod.mid(arrow + 2).trimmed();
            // 计算实际弹栈数：k = |stack_prev| - |stack_curr| + 1
            int prevSize = (i > 0) ? r.steps[i - 1].stack.size() : ps.stack.size();
            int currSize = ps.stack.size();
            int kCalc    = prevSize - currSize + 1;
            int kRhs     = (R == Config::epsilonSymbol() || R.isEmpty())
                               ? 0
                               : R.split(' ', Qt::SkipEmptyParts).size();
            int k        = (kCalc >= 0) ? kCalc : kRhs;
            int rid      = redIdx.value(QString("%1 -> %2").arg(L).arg(R), -1);
            if (rid >= 0)
                desc = QStringLiteral("归约：用产生式 r%1：%2 -> %3，弹出%4个符号")
                           .arg(rid)
                           .arg(L)
                           .arg(R)
                           .arg(k);
            else
                desc = QStringLiteral("归约：用产生式 %1，弹出%2个符号").arg(prod).arg(k);
        }
        else if (ps.action == "acc")
        {
            desc = QStringLiteral("接受：分析完成");
        }
        else
        {
            desc = QStringLiteral("动作：%1").arg(ps.action);
        }
        auto c3 = new QTableWidgetItem(desc);
        if (ps.action == "error")
        {
            c0->setForeground(Qt::red);
            c1->setForeground(Qt::red);
            c2->setForeground(Qt::red);
            c3->setForeground(Qt::red);
        }
        tbl_->setItem(i, 0, c0);
        tbl_->setItem(i, 1, c1);
        tbl_->setItem(i, 2, c2);
        tbl_->setItem(i, 3, c3);
    }
    tbl_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tbl_->setWordWrap(true);
    tbl_->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // 先按内容自适应，记录当前列宽
    tbl_->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    int wStep  = tbl_->horizontalHeader()->sectionSize(0);
    int wStack = tbl_->horizontalHeader()->sectionSize(1);
    int wInput = tbl_->horizontalHeader()->sectionSize(2);
    int wDesc  = tbl_->horizontalHeader()->sectionSize(3);
    // 切换为固定列宽并按要求缩放
    tbl_->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tbl_->setColumnWidth(0, qMax(40, wStep / 3));
    tbl_->setColumnWidth(1, qMax(40, wStack / 3));
    tbl_->setColumnWidth(2, 420);
    tbl_->setColumnWidth(3, wDesc);
    v->addWidget(tbl_);
    resize(1000, 600);
}

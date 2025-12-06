#pragma once
#include <QDialog>
#include <QTableWidget>
#include "../../../src/syntax/LR1.h"
#include "../../../src/syntax/LR1Parser.h"

class GrammarProcessDialog : public QDialog
{
    Q_OBJECT
   public:
    GrammarProcessDialog(const ParseResult&    r,
                         const LR1ActionTable& tbl,
                         QWidget*              parent = nullptr);

   private:
    QTableWidget*  tbl_ = nullptr;
    static QString formatStack(const QVector<QPair<int, QString>>& stack);
    static QString formatRest(const QVector<QString>& rest);
};

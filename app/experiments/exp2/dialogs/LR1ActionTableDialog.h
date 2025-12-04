#pragma once
#include <QDialog>
#include <QTableWidget>
#include "../../../src/syntax/LR1.h"
class LR1ActionTableDialog : public QDialog
{
    Q_OBJECT
   public:
    explicit LR1ActionTableDialog(const Grammar&        g,
                                  const LR1ActionTable& t,
                                  QWidget*              parent = nullptr);
};

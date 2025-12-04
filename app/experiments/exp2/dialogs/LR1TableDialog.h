#pragma once
#include <QDialog>
#include <QTableWidget>
#include "../../../src/syntax/LR1.h"
class LR1TableDialog : public QDialog
{
    Q_OBJECT
   public:
    explicit LR1TableDialog(const LR1Graph& gr, QWidget* parent = nullptr);
};

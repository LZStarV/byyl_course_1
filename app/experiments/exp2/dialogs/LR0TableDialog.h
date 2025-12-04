#pragma once
#include <QDialog>
#include <QTableWidget>
#include "../../../src/syntax/LR0.h"
class LR0TableDialog : public QDialog
{
    Q_OBJECT
   public:
    explicit LR0TableDialog(const LR0Graph& gr, QWidget* parent = nullptr);
};

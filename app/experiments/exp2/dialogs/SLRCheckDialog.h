#pragma once
#include <QDialog>
#include "../../../src/syntax/SLR.h"
class QLabel;
class QPlainTextEdit;
class SLRCheckDialog : public QDialog
{
    Q_OBJECT
   public:
    explicit SLRCheckDialog(const SLRCheckResult& r, QWidget* parent = nullptr);
};

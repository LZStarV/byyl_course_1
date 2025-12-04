#pragma once
#include <QWidget>
class QPushButton;
class QLineEdit;
class LRTab : public QWidget
{
    Q_OBJECT
   public:
    explicit LRTab(QWidget* parent = nullptr);
    QPushButton* btnExportLR0;
    QPushButton* btnPreviewLR0;
    QLineEdit*   edtGraphDpiLR0;
    QPushButton* btnViewLR0Table;
    QPushButton* btnCheckSLR1;
    QPushButton* btnExportLR1;
    QPushButton* btnPreviewLR1;
    QLineEdit*   edtGraphDpiLR1;
    QPushButton* btnViewLR1Table;
    QPushButton* btnViewLR1Action;
};

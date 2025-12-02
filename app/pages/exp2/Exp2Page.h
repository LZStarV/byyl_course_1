#pragma once
#include <QWidget>
class QPushButton;
class QLabel;
class Exp2Page : public QWidget
{
    Q_OBJECT
   public:
    explicit Exp2Page(QWidget* parent = nullptr);
   signals:
    void requestBack();
   private:
    QPushButton* btnBack;
    QLabel*      lblDev;
};


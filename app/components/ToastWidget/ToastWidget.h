/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：ToastWidget.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#ifndef TOASTWIDGET_H
/* 用途：宏定义占位说明（请补充用途与参数） */
#define TOASTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QGraphicsDropShadowEffect>

class ToastWidget : public QWidget
{
    Q_OBJECT
   public:
    enum Type
    {
        Info,
        Warning,
        Error
    };
    explicit ToastWidget(Type type, QWidget* parent = nullptr);
    void setText(const QString& t);
    void setDuration(int ms);
    void setIcon(const QIcon& ico);
    void showAnimated();
   public slots:
    void closeAnimated();
   signals:
    void closed(ToastWidget* self);

   protected:
    void resizeEvent(QResizeEvent* e) override;

   private:
    void                       applyType(Type type);
    void                       ensureEffects();
    QLabel*                    icon_;
    QLabel*                    text_;
    QPushButton*               close_;
    QTimer*                    timer_;
    QPropertyAnimation*        fade_;
    QPropertyAnimation*        slide_;
    QGraphicsOpacityEffect*    opacity_;
    QGraphicsDropShadowEffect* shadow_;
    int                        durationMs_;
};

class InfoToast : public ToastWidget
{
   public:
    explicit InfoToast(QWidget* parent = nullptr) : ToastWidget(ToastWidget::Info, parent) {}
};
class WarningToast : public ToastWidget
{
   public:
    explicit WarningToast(QWidget* parent = nullptr) : ToastWidget(ToastWidget::Warning, parent) {}
};
class ErrorToast : public ToastWidget
{
   public:
    explicit ErrorToast(QWidget* parent = nullptr) : ToastWidget(ToastWidget::Error, parent) {}
};

#endif
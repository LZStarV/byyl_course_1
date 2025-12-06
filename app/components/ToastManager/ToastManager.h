/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：ToastManager.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#ifndef TOASTMANAGER_H
/* 用途：宏定义占位说明（请补充用途与参数） */
#define TOASTMANAGER_H

#include <QObject>
#include <QQueue>
#include <QPointer>
#include <QRect>
#include <QWidget>
#include "../ToastWidget/ToastWidget.h"

struct ToastItem
{
    ToastWidget::Type type;
    QString           text;
    int               duration;
};

class ToastManager : public QObject
{
    Q_OBJECT
   public:
    static ToastManager& instance();
    void                 setAnchor(QWidget* w);
    void                 showInfo(const QString& text);
    void                 showWarning(const QString& text);
    void                 showError(const QString& text);
    void                 setLimit(int n);
    void                 setMargins(int right, int top);
    void                 setSpacing(int s);

   private:
    ToastManager();
    void  enqueue(ToastWidget::Type t, const QString& text, int ms);
    void  tryShowNext();
    void  layoutToasts();
    QRect anchorAvailableGeometry() const;

   private:
    QPointer<QWidget>   anchor_;
    QList<ToastWidget*> active_;
    QQueue<ToastItem>   queue_;
    int                 limit_;
    int                 marginRight_;
    int                 marginTop_;
    int                 spacing_;
    int                 width_;
};

#endif
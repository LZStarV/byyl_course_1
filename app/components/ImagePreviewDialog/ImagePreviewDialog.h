/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：ImagePreviewDialog.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#pragma once
#include <QDialog>
class QGraphicsView;
class QGraphicsScene;
class QPushButton;

class ImagePreviewDialog : public QDialog
{
    Q_OBJECT
   public:
    explicit ImagePreviewDialog(QWidget* parent = nullptr);
    bool loadImage(const QString& pngPath);

   private:
    QGraphicsView*  view_;
    QGraphicsScene* scene_;
    QPushButton*    btnZoomIn_;
    QPushButton*    btnZoomOut_;
    QPushButton*    btnFit_;
    QPushButton*    btnReset_;
};
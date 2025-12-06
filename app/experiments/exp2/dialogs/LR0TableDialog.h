/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LR0TableDialog.h
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
#include <QTableWidget>
#include "../../../../src/syntax/LR0.h"
class LR0TableDialog : public QDialog
{
    Q_OBJECT
   public:
    explicit LR0TableDialog(const LR0Graph& gr, QWidget* parent = nullptr);
};
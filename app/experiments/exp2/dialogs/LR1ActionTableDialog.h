/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：LR1ActionTableDialog.h
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
#include "../../../../src/syntax/LR1.h"
class LR1ActionTableDialog : public QDialog
{
    Q_OBJECT
   public:
    explicit LR1ActionTableDialog(const Grammar&        g,
                                  const LR1ActionTable& t,
                                  QWidget*              parent = nullptr);
};
/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：NFAViewTab.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "NFAViewTab.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include "../../../../components/ExportGraphButton/ExportGraphButton.h"

NFAViewTab::NFAViewTab(QWidget* parent) : QWidget(parent)
{
    auto l    = new QVBoxLayout(this);
    auto hSel = new QHBoxLayout;
    auto lbl  = new QLabel("选择Token");
    cmbTokens = new QComboBox;
    cmbTokens->setObjectName("cmbTokens");
    hSel->addWidget(lbl);
    hSel->addWidget(cmbTokens);
    auto hTools  = new QHBoxLayout;
    btnExportNFA = new ExportGraphButton("导出(NFA)");
    btnExportNFA->setObjectName("btnExportNFA");
    btnPreviewNFA = new QPushButton("预览(NFA)");
    btnPreviewNFA->setObjectName("btnPreviewNFA");
    edtGraphDpiNfa = new QLineEdit;
    edtGraphDpiNfa->setObjectName("edtGraphDpiNfa");
    edtGraphDpiNfa->setPlaceholderText("DPI(默认150)");
    hTools->addWidget(btnExportNFA);
    hTools->addWidget(btnPreviewNFA);
    hTools->addWidget(new QLabel("分辨率DPI"));
    hTools->addWidget(edtGraphDpiNfa);
    tblNFA = new QTableWidget;
    tblNFA->setObjectName("tblNFA");
    l->addLayout(hSel);
    l->addLayout(hTools);
    l->addWidget(tblNFA);
}
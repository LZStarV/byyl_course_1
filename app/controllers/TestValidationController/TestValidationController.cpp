/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：TestValidationController.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include "TestValidationController.h"
#include <QWidget>
#include <QPushButton>
#include "../../mainwindow.h"

TestValidationController::TestValidationController(MainWindow* mw) : mw_(mw) {}

void TestValidationController::bind(QWidget* testPage)
{
    auto btnPick   = testPage->findChild<QPushButton*>("btnPickSample");
    auto btnRun    = testPage->findChild<QPushButton*>("btnRunLexer");
    auto btnSaveAs = testPage->findChild<QPushButton*>("btnSaveLexResultAs");
    if (btnPick)
        QObject::connect(btnPick, &QPushButton::clicked, mw_, &MainWindow::pickSample);
    if (btnRun)
        QObject::connect(btnRun, &QPushButton::clicked, mw_, &MainWindow::runLexer);
    if (btnSaveAs)
        QObject::connect(btnSaveAs, &QPushButton::clicked, mw_, &MainWindow::saveLexAs);
}
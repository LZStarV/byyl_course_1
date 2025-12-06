/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：SyntaxController.h
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#pragma once
#include <QObject>
#include <QString>
#include <QVector>
#include <QLineEdit>
#include <QTableWidget>
#include <QGraphicsView>
#include "../../../src/syntax/Grammar.h"
#include "../../../src/syntax/LL1.h"
class QWidget;
class QTableWidget;
class QTextEdit;
class QPushButton;
class MainWindow;
class Engine;
struct LL1Info;
class NotificationService;
class DotService;

class SyntaxController : public QObject
{
    Q_OBJECT
   public:
    SyntaxController(MainWindow* mw, Engine* engine, NotificationService* notify);
    void bind(QWidget* exp2Page);
    void loadGrammar();
    void parseGrammar();
    void runSyntaxAnalysis();
    void exportDot();
    void previewTree();
    void previewLR0();
    void exportLR0Dot();
    void exportAstDot();
    void checkSLR1();
    void exportLR1Dot();
    void previewLR1();
    void openLR1Table();
    void openLR0Table();

   private:
    MainWindow*          mw_;
    Engine*              engine_;
    NotificationService* notify_;
    DotService*          dotSvc_ = nullptr;
    QWidget*             page_   = nullptr;
    struct Grammar       grammar_;
    struct LL1Info       ll1_;
    bool                 hasGrammar_ = false;
    static bool renderDotFromContentLocal(const QString& dotContent, QString& outPngPath, int dpi);
};
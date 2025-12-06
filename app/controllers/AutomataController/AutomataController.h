/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：AutomataController.h
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
class QWidget;
class QComboBox;
class QTableWidget;
class MainWindow;
class DotService;
struct Tables;

class AutomataController : public QObject
{
    Q_OBJECT
   public:
    explicit AutomataController(MainWindow* mw);
    void bind(QWidget* root);

   private:
    MainWindow*   mw_;
    QWidget*      root_      = nullptr;
    DotService*   dot_       = nullptr;
    QComboBox*    cmbTok_    = nullptr;
    QComboBox*    cmbTokDfa_ = nullptr;
    QComboBox*    cmbTokMin_ = nullptr;
    QTableWidget* tblNfa_    = nullptr;
    QTableWidget* tblDfa_    = nullptr;
    QTableWidget* tblMin_    = nullptr;
    void          onTokenChanged(int idx);
    void          onTokenChangedDFA(int idx);
    void          onTokenChangedMin(int idx);
    void          fillTable(QTableWidget* tbl, const Tables& t);
    void          fillAllNFA();
    void          fillAllDFA();
    void          fillAllMin();
    void          exportNfaDot();
    void          exportNfaImage();
    void          previewNfa();
    void          exportDfaDot();
    void          exportDfaImage();
    void          previewDfa();
    void          exportMinDot();
    void          exportMinImage();
    void          previewMin();
};
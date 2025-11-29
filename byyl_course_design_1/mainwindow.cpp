#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStatusBar>
#include <QDesktopServices>
#include <QUrl>
#include <QTabWidget>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QLabel>
#include <QProcess>
#include <QDir>
#include "../src/core/Engine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    engine = new Engine();
    setupUiCustom();
}

MainWindow::~MainWindow()
{
    delete engine;
    delete ui;
}

void MainWindow::setupUiCustom(){
    tabs = new QTabWidget(ui->centralwidget);
    auto v = new QVBoxLayout; ui->centralwidget->setLayout(v); v->addWidget(tabs);
    auto w1 = new QWidget; auto l1 = new QVBoxLayout(w1); txtInputRegex = new QTextEdit; txtInputRegex->setObjectName("txtInputRegex");
    auto h1 = new QHBoxLayout; btnLoadRegex = new QPushButton("从文件加载"); btnLoadRegex->setObjectName("btnLoadRegex"); btnSaveRegex = new QPushButton("保存正则"); btnSaveRegex->setObjectName("btnSaveRegex"); btnStartConvert = new QPushButton("转换"); btnStartConvert->setObjectName("btnStartConvert"); h1->addWidget(btnLoadRegex); h1->addWidget(btnSaveRegex); h1->addWidget(btnStartConvert);
    l1->addWidget(txtInputRegex); l1->addLayout(h1); tabs->addTab(w1, "正则编辑");
    auto w2 = new QWidget; auto l2 = new QVBoxLayout(w2); tblNFA = new QTableWidget; tblNFA->setObjectName("tblNFA"); l2->addWidget(tblNFA); tabs->addTab(w2, "NFA状态表");
    auto w3 = new QWidget; auto l3 = new QVBoxLayout(w3); tblDFA = new QTableWidget; tblDFA->setObjectName("tblDFA"); l3->addWidget(tblDFA); tabs->addTab(w3, "DFA状态表");
    auto w4 = new QWidget; auto l4 = new QVBoxLayout(w4); tblMinDFA = new QTableWidget; tblMinDFA->setObjectName("tblMinDFA"); btnGenCode = new QPushButton("生成代码"); btnGenCode->setObjectName("btnGenCode"); l4->addWidget(tblMinDFA); l4->addWidget(btnGenCode); tabs->addTab(w4, "最小化DFA");
    auto w5 = new QWidget; auto l5 = new QVBoxLayout(w5); txtGeneratedCode = new QPlainTextEdit; txtGeneratedCode->setObjectName("txtGeneratedCode"); btnCompileRun = new QPushButton("编译并运行生成器"); btnCompileRun->setObjectName("btnCompileRun"); l5->addWidget(txtGeneratedCode); l5->addWidget(btnCompileRun); tabs->addTab(w5, "代码查看");
    auto w6 = new QWidget; auto l6 = new QVBoxLayout(w6);
    auto h6 = new QHBoxLayout;
    auto leftCol = new QVBoxLayout; auto rightCol = new QVBoxLayout;
    auto lblSrc = new QLabel("源程序输入（JS/TINY 片段）");
    auto lblOut = new QLabel("Token 编码输出");
    txtSourceTiny = new QPlainTextEdit; txtSourceTiny->setObjectName("txtSourceTiny");
    txtLexResult = new QPlainTextEdit; txtLexResult->setObjectName("txtLexResult");
    leftCol->addWidget(lblSrc); leftCol->addWidget(txtSourceTiny);
    rightCol->addWidget(lblOut); rightCol->addWidget(txtLexResult);
    h6->addLayout(leftCol); h6->addLayout(rightCol);
    btnRunLexer = new QPushButton("运行词法分析"); btnRunLexer->setObjectName("btnRunLexer");
    l6->addLayout(h6); l6->addWidget(btnRunLexer);
    tabs->addTab(w6, "测试与验证");
    connect(btnStartConvert,&QPushButton::clicked,this,&MainWindow::onConvertClicked);
    connect(btnGenCode,&QPushButton::clicked,this,&MainWindow::onGenCodeClicked);
    connect(btnRunLexer,&QPushButton::clicked,this,&MainWindow::onRunLexerClicked);
    connect(btnCompileRun,&QPushButton::clicked,this,&MainWindow::onCompileRunClicked);
    connect(btnLoadRegex,&QPushButton::clicked,this,&MainWindow::onLoadRegexClicked);
    connect(btnSaveRegex,&QPushButton::clicked,this,&MainWindow::onSaveRegexClicked);
}

void MainWindow::fillTable(QTableWidget* tbl, const Tables& t){
    tbl->clear(); tbl->setColumnCount(t.columns.size()); tbl->setRowCount(t.rows.size());
    QStringList headers; for(auto c: t.columns) headers<<c; tbl->setHorizontalHeaderLabels(headers);
    for(int r=0;r<t.rows.size();++r){ auto row=t.rows[r]; for(int c=0;c<row.size();++c){ tbl->setItem(r,c,new QTableWidgetItem(row[c])); } }
    tbl->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::onConvertClicked(bool){
    auto text = txtInputRegex->toPlainText(); auto rf = engine->lexFile(text); auto parsed = engine->parseFile(rf);
    if(parsed.tokens.isEmpty()){ statusBar()->showMessage("未找到Token定义"); return; }
    parsedPtr = new ParsedFile(parsed);
    auto pt = parsed.tokens.first(); auto nfa = engine->buildNFA(pt.ast, parsed.alpha); auto dfa = engine->buildDFA(nfa); auto mdfa = engine->buildMinDFA(dfa); lastMinPtr = new MinDFA(mdfa);
    auto tn = engine->nfaTable(nfa); fillTable(tblNFA, tn);
    auto td = engine->dfaTable(dfa); fillTable(tblDFA, td);
    auto tm = engine->minTable(mdfa); fillTable(tblMinDFA, tm);
    statusBar()->showMessage("转换成功");
}

void MainWindow::onGenCodeClicked(bool){
    if(!lastMinPtr){ statusBar()->showMessage("请先转换"); return; }
    auto parsed = *parsedPtr; QMap<QString,int> codes; for(auto p: parsed.tokens){ codes[p.rule.name]=p.rule.code; }
    auto s = engine->generateCode(*lastMinPtr, codes); txtGeneratedCode->setPlainText(s); statusBar()->showMessage("代码已生成");
}

void MainWindow::onRunLexerClicked(bool){
    if(!parsedPtr){ auto text = txtInputRegex->toPlainText(); auto rf = engine->lexFile(text); auto parsed = engine->parseFile(rf); if(parsed.tokens.isEmpty()){ statusBar()->showMessage("未找到Token定义"); return; } parsedPtr = new ParsedFile(parsed); }
    auto src = txtSourceTiny->toPlainText();
    if(src.trimmed().isEmpty()){
        QString p1 = QCoreApplication::applicationDirPath()+"/../../resources/sample.tny";
        QFile f1(p1);
        if(f1.open(QIODevice::ReadOnly|QIODevice::Text)){ QTextStream in(&f1); src = in.readAll(); f1.close(); }
        else {
            QString p2 = QCoreApplication::applicationDirPath()+"/resources/sample.tny";
            QFile f2(p2);
            if(f2.open(QIODevice::ReadOnly|QIODevice::Text)){ QTextStream in(&f2); src = in.readAll(); f2.close(); }
            else {
                QFile f3("resources/sample.tny");
                if(f3.open(QIODevice::ReadOnly|QIODevice::Text)){ QTextStream in(&f3); src = in.readAll(); f3.close(); }
            }
        }
        if(src.trimmed().isEmpty()){
            src = QStringLiteral("abc123 def456\nif return == var abc123\n");
            statusBar()->showMessage("未检测到输入文本，已注入示例文本");
        }
        txtSourceTiny->setPlainText(src);
    }
    QVector<int> codes; auto mdfas = engine->buildAllMinDFA(*parsedPtr, codes); auto res = engine->runMultiple(mdfas, codes, src);
    txtLexResult->setPlainText(res);
    QString outPath = QCoreApplication::applicationDirPath()+"/sample.lex";
    QFile of(outPath);
    if(of.open(QIODevice::WriteOnly|QIODevice::Text)){ QTextStream o(&of); o << res << "\n"; of.close(); }
    if(res.contains("ERR")) { statusBar()->showMessage("存在未识别的词法单元(ERR)，请检查正则与输入"); }
    else { statusBar()->showMessage("测试完成"); }
}

void MainWindow::onSaveRegexClicked(bool){
    auto path = QFileDialog::getSaveFileName(this, QStringLiteral("保存正则为"), QString(), QStringLiteral("Text (*.txt *.regex);;All (*)"));
    if(path.isEmpty()) return;
    QFile f(path);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text)){ statusBar()->showMessage("文件保存失败"); return; }
    QTextStream out(&f); out << txtInputRegex->toPlainText(); f.close(); statusBar()->showMessage("正则已保存");
}

void MainWindow::onLoadRegexClicked(bool){
    auto path = QFileDialog::getOpenFileName(this, QStringLiteral("选择正则文件"), QString(), QStringLiteral("Text (*.txt *.regex);;All (*)"));
    if(path.isEmpty()) return;
    QFile f(path);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text)){ statusBar()->showMessage("文件打开失败"); return; }
    QTextStream in(&f); auto content = in.readAll(); txtInputRegex->setPlainText(content); statusBar()->showMessage("正则已加载");
}
void MainWindow::onCompileRunClicked(bool){
    if(!parsedPtr){ auto text = txtInputRegex->toPlainText(); auto rf = engine->lexFile(text); auto parsed = engine->parseFile(rf); if(parsed.tokens.isEmpty()){ statusBar()->showMessage("未找到Token定义"); return; } parsedPtr = new ParsedFile(parsed); }
    auto parsed = *parsedPtr; QMap<QString,int> codesMap; for(auto p: parsed.tokens){ codesMap[p.rule.name]=p.rule.code; }
    auto pt = parsed.tokens.first(); auto nfa = engine->buildNFA(pt.ast, parsed.alpha); auto dfa = engine->buildDFA(nfa); auto mdfa = engine->buildMinDFA(dfa);
    auto core = CodeGenerator::generate(mdfa, codesMap);
    int codeNum = pt.rule.code;
    QString src;
    src += "#include <iostream>\n#include <string>\n";
    src += core + "\n";
    src += "int main(){\n";
    src += "  std::string input;\n";
    src += "  std::string line;\n";
    src += "  while(std::getline(std::cin,line)){ if(!input.empty()) input+='\\n'; input+=line; }\n";
    src += "  std::string out;\n";
    src += "  size_t pos=0;\n";
    src += "  while(pos<input.size()){\n";
    src += "    char c=input[pos++];\n";
    src += "    if(c==' '||c=='\\t'||c=='\\n'||c=='\\r'){ continue; }\n";
    src += "    if(c=='{'){ while(pos<input.size() && input[pos++]!='}'){} continue; }\n";
    src += "    int state=";
    src += QString::number(mdfa.start);
    src += ";\n";
    src += "    bool moved=true;\n";
    src += "    while(moved){\n";
    src += "      int ns=Step(state,c);\n";
    src += "      if(ns==-1){ moved=false; break; }\n";
    src += "      state=ns;\n";
    src += "      if(pos<input.size()) c=input[pos++]; else c='\\0';\n";
    src += "      if(c=='\\0') break;\n";
    src += "    }\n";
    src += "    if(AcceptState(state)){\n";
    src += "      if(!out.empty()) out+=' ';\n";
    src += "      out+=std::to_string(";
    src += QString::number(codeNum);
    src += ");\n";
    src += "    } else {\n";
    src += "      if(!out.empty()) out+=' ';\n";
    src += "      out+=std::string(\"ERR\");\n";
    src += "    }\n";
    src += "  }\n";
    src += "  std::cout<<out;\n";
    src += "  return 0;\n";
    src += "}\n";
    QString outCpp = QCoreApplication::applicationDirPath()+"/gen_lex_ui.cpp";
    QFile f(outCpp); if(!f.open(QIODevice::WriteOnly|QIODevice::Text)){ statusBar()->showMessage("生成代码写入失败"); return; } QTextStream w(&f); w<<src; f.close();
    QProcess proc; QString bin = QCoreApplication::applicationDirPath()+"/gen_lex_ui_bin"; proc.start("clang++", QStringList()<<"-std=c++17"<<outCpp<<"-o"<<bin); proc.waitForFinished(); if(proc.exitStatus()!=QProcess::NormalExit || proc.exitCode()!=0){ txtLexResult->setPlainText(QString::fromUtf8(proc.readAllStandardError())); statusBar()->showMessage("编译失败"); return; }
    QString inputText = txtSourceTiny->toPlainText(); if(inputText.trimmed().isEmpty()){ QString p1 = QCoreApplication::applicationDirPath()+"/../../resources/sample.tny"; QFile f1(p1); if(f1.open(QIODevice::ReadOnly|QIODevice::Text)){ QTextStream in(&f1); inputText = in.readAll(); f1.close(); } else { QString p2 = QCoreApplication::applicationDirPath()+"/resources/sample.tny"; QFile f2(p2); if(f2.open(QIODevice::ReadOnly|QIODevice::Text)){ QTextStream in(&f2); inputText = in.readAll(); f2.close(); } else { QFile f3("resources/sample.tny"); if(f3.open(QIODevice::ReadOnly|QIODevice::Text)){ QTextStream in(&f3); inputText = in.readAll(); f3.close(); } } }
    QProcess run; run.start(bin); run.write(inputText.toUtf8()); run.closeWriteChannel(); run.waitForFinished(); auto output = QString::fromUtf8(run.readAllStandardOutput()); txtLexResult->setPlainText(output); QString outLex = QCoreApplication::applicationDirPath()+"/sample.lex"; QFile lf(outLex); if(lf.open(QIODevice::WriteOnly|QIODevice::Text)){ QTextStream o(&lf); o<<output<<"\n"; lf.close(); }
    statusBar()->showMessage("生成器运行完成");
}

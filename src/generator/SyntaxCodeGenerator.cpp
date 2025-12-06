/*
 * 版权信息：Copyright (c) 2023 林展星
 * 文件名称：SyntaxCodeGenerator.cpp
 *
 * 当前版本：1.0.0
 * 作    者：林展星
 * 完成日期：2023年12月07日
 *
 * 版本历史：
 * 1.0.0 2023-12-07 林展星 初始版本
 */
#include <QString>
#include <QMap>
#include <QVector>

QString generateSyntaxParserSource(const QMap<QString, QMap<QString, int>>& table,
                                   const QVector<QString>&                  nonterms,
                                   const QVector<QString>&                  terms,
                                   const QString&                           start)
{
    QString s;
    s += "#include <iostream>\n#include <vector>\n#include <string>\n#include <unordered_map>\n";
    s += "int main() { std::cout << \"syntax parser stub\"; return 0; }\n";
    return s;
}
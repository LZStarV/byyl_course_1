# 07 词法代码生成（CodeGenerator）

## 数据结构

表 1 数据结构总览

对象或变量名称 | 数据结构 | 存储结构用途
| - | - | -
Judgechar | 函数 | 将字符按字母/数字分类，返回类别码（letter→1，digit→0，其它→-1）
AcceptState / AcceptState_i | 函数 | 判定状态是否为接受态（单 DFA / 组合模式分别生成）
Step / Step_i | 函数 | 按 `switch(state)` 与字面字符匹配执行一步状态转移（单/多 DFA）
StepFn / AcceptFn | 类型别名 | 函数指针类型：`int (*)(int,char)` 与 `bool (*)(int)`
STEPS / ACCEPTS / STARTS | 数组 | 每个 DFA 的步进函数、接受判定函数与起始态编号分发表
IDENT_CODES | 数组 | 标识符编码集合（升序），用于决定是否追加词素
isIdentifierCode | 函数 | 判断编码是否属于标识符类（线性扫描 `IDENT_CODES`）
emitIdLex | 函数 | 是否追加词素（读取 `LEXER_EMIT_IDENTIFIER_LEXEME` 环境变量）
codeWeight | 函数 | 等长匹配的权重函数（解析 `LEXER_WEIGHTS`，内部 `mins[]/ws[]/cnt`）
matchLen | 函数 | 计算指定 DFA 在位置 `pos` 的最长可接受前缀长度（用 `STARTS/STEPS/ACCEPTS`）
codeList | 数组 | 组合模式下的编码列表，用于权重与选择规则（长度>权重>索引）
输出源码 | 文本 | 生成的 C++ 源码字符串（包含主程序、读入与打印输出）

## 算法实现过程

- 单 DFA 源码生成：遍历最小化 DFA 的状态与转移，生成 `AcceptState/Step` 的 `switch` 结构；按字母表配置生成 `Judgechar`。
- 组合源码生成：为每个最小化 DFA 生成同名函数后缀 `_i`；构建分发表与标识符编码集合；嵌入匹配长度计算与权重比较逻辑。
- 分类与步进：`Judgechar` 按 `hasLetter/hasDigit` 返回类别；`AcceptState/Step` 以 `switch(state)` 构造接受与转移；
- 分发表与集合：`STEPS/ACCEPTS/STARTS` 固定数组按 DFA 数量初始化；`IDENT_CODES` 与 `emitIdLex` 控制词素追加；
- 权重策略：环境变量 `LEXER_WEIGHTS` 格式 `minCode:weight,...`，按 `minCode` 降序排序；等长时比较权重与索引；
- 跳过策略：行/块/井号注释与各类字符串的跳过在主扫描循环处理；

下图展示代码查看页签中的关键函数片段（Step/AcceptState 与分发表）：
<!--【GUI 截图】此处需要添加 '代码查看页签截图'：展示生成的 Step/AcceptState 片段与分发表。 -->

## 算法实现过程（编号式）

1. 读取输入（MinDFA 列表与编码列表/映射），准备字母表类别与标识符编码集合（可配置词素追加）。
2. 生成分类函数 `Judgechar`：按字母与数字类别返回分类码，其余返回 -1。
3. 生成接受判定与步进函数：
   3.1 单 DFA：`AcceptState/Step` 两函数；
   3.2 组合模式：为每个 DFA 生成 `AcceptState_i/Step_i`；
4. 构建分发表与起始态数组：`STEPS/ACCEPTS/STARTS`；构建 `IDENT_CODES` 与 `emitIdLex` 开关函数。
5. 嵌入权重策略：解析 `LEXER_WEIGHTS` 并建立比较逻辑（长度优先，长度相同时权重优先，再索引稳定）。
6. 主程序包装：读取输入文本、循环扫描、应用跳过策略与匹配决策，输出编码序列并可追加词素。

下图为“词法代码生成管线流程图”占位：
<!--【流程图】此处需要添加 '代码生成管线流程图'：状态遍历→函数生成→分发表→权重与跳过→主程序。 -->

## 各种情况讲解

- 字母表配置缺失：若未启用 `hasLetter/hasDigit`，分类函数仍安全退化；
- 标识符词素输出开关：可通过环境变量关闭 `emitIdLex`；
- 权重配置为空：采用默认权重表，保障优先级稳定。

## 关键代码片段

```
// 单 DFA 源码生成（示意）：遍历状态/转移生成 AcceptState/Step 与 Judgechar
QString CodeGenerator::generate(const MinDFA& mdfa, const QMap<QString,int>& tokenCodes);

// 组合扫描器源码生成：生成 AcceptState_i/Step_i，分发表与匹配逻辑
QString CodeGenerator::generateCombined(const QVector<MinDFA>& mdfas,
                                        const QVector<int>&    codes,
                                        const Alphabet&        alpha,
                                        const QSet<int>&       identifierCodes)
{
    // 分类函数：按配置的字母与数字返回类别码
    // static inline int Judgechar(char ch) { if (isalpha(ch)) return 1; if (isdigit(ch)) return 0; return -1; }

    // 为每个 DFA 生成 AcceptState_i/Step_i（switch(state) + 字面字符匹配）
    // genAcceptStateI(mdfas[i], i); genStepI(mdfas[i], i);

    // 分发表与起始态数组
    // typedef int (*StepFn)(int,char); typedef bool (*AcceptFn)(int);
    // static StepFn STEPS[N] = { Step_0, Step_1, ... };
    // static AcceptFn ACCEPTS[N] = { AcceptState_0, AcceptState_1, ... };
    // static int STARTS[N] = { start0, start1, ... };

    // 等长匹配权重：从环境变量 LEXER_WEIGHTS 解析，按 minCode 降序匹配
    // static inline int codeWeight(int c) { /* parse env -> mins[], ws[]; default fallback; pick first matched */ }

    // 最长匹配：对指定 DFA 自 pos 起步进，记录最后接受位置
    // static inline int matchLen(int idx, const string& src, size_t pos) {
    //     int s = STARTS[idx], last = -1; size_t p = pos; StepFn step = STEPS[idx]; AcceptFn acc = ACCEPTS[idx];
    //     while (p < src.size()) { int ns = step(s, src[p]); if (ns == -1) break; s = ns; if (acc(s)) last = ++p; else ++p; }
    //     return last == -1 ? 0 : int(last - pos);
    // }

    // 主程序：遍历所有 DFA 计算 matchLen，按“长度>权重>索引”选择最优；
    // 若命中 identifierCodes 且 emitIdLex 开启，则追加词素；否则仅输出编码；失败输出 ERR 并前进 1。
}
```

## 单元测试

输入：
- 目标 Token：`_identifier100`；表达式：`letter (letter | digit)*`；宏：`letter=[A-Za-z_]`、`digit=[0-9]`
- 构建最小化 DFA 并调用代码生成；随后以 `clang++` 编译并运行生成的扫描器；

预测结果：
- 生成的源代码非空，编译正常退出；
- 运行时对输入如 `abc123 def456` 输出非空的编码序列，词法状态机正常接受；

测试结果：
- 控制台输出包含“生成器编译退出码”“编译器错误输出”“生成器运行输出”等信息；
- 退出码为 0 且运行输出非空，验证生成器产物可编译并正确运行；
- GUI 截图：代码生成编译运行单元测试（CodegenTest）的运行截图（含编译退出码与运行输出）。
<!--【GUI 截图】此处需要添加 'CodegenTest 运行截图'：显示编译退出码与运行输出概要。 -->
如需辅助展示，可在代码查看页签处提供“关键函数片段与分发表”的截图。

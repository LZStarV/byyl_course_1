# 编译原理课程设计任务一：词法分析生成器

## 1. 任务概述
设计并实现一个**窗口式图形界面（GUI）应用程序**，该程序能够读取高级程序设计语言的**正则表达式**定义，自动进行一系列转换（NFA $\to$ DFA $\to$ 最小化DFA），最终生成该语言的**C/C++词法分析源代码**。

## 2. 开发环境与基本要求
*   **编程语言**：必须使用 **C** 或 **C++**。
*   **界面形式**：必须是**窗口式图形界面**（QT），不能是纯控制台程序。

## 3. 核心功能模块

系统需包含以下核心功能，并提供相应的可视化界面：

### 3.1 正则表达式编辑与输入
*   **文件操作**：支持打开和保存包含正则表达式的文本文件。
*   **编辑界面**：提供编辑框供用户输入一行或多行正则表达式。
*   **支持的运算符**：
    *   **连接**：直接书写（如 `ab`）
    *   **选择**：`|`
    *   **闭包**：`*`
    *   **正闭包**：`+`
    *   **可选**：`?`
    *   **字符集**：`[]` (如 `[a-z]`)
    *   **括号**：`()`
    *   **转义符号**：`\` (特别注意：由于 `+` 和 `*` 被用作运算符号，如果需要表示算术中的加号或乘号，必须使用 `\+` 和 `\*`)。
*   **支持命名操作**：允许定义宏（如 `digit=[0-9]`）。

### 3.2 自动机转换与可视化
系统需要展示转换过程中的中间状态：
1.  **RE $\to$ NFA**：展示转换后的非确定有限自动机。
2.  **NFA $\to$ DFA**：展示确定化后的有限自动机。
3.  **DFA $\to$ Min-DFA**：展示最小化后的有限自动机。

系统需要将正则表达式转换为词法分析器的过程可视化。要求使用**状态转换表**的形式来展示中间结果。表格的通用结构必须严格遵守以下规范：

*   **第一列（标记列）**：用于标识状态类型。`-` 表示初态（Start State），`+` 表示终态（Accepting State）。
*   **第二列（状态列）**：显示状态的唯一标识符（ID）或状态集合（Set）。
*   **后续列（输入字符列）**：每一列对应正则表达式中出现的一个输入符号（如 `letter`, `digit` 等）或 $\epsilon$（通常用 `#` 表示）。**程序需要动态识别并生成这些列**。
*   **表格内容**：显示在特定输入下跳转到的目标状态或状态集。

以下是三个阶段的具体展示要求与数据示例：

#### (1) RE $\to$ NFA (正则表达式转非确定有限自动机)
展示 Thompson 算法构造出的 NFA 状态转换表。必须包含 $\epsilon$ 转换（用 `#` 表示）。

**示例数据与表现形式：**

| 标记  | 状态 ID | letter | digit | # (Epsilon) |
| :---: | :-----: | :----: | :---: | :---------: |
| **-** |  **1**  |   2    |       |             |
|       |  **2**  |        |       |      3      |
|       |  **3**  |        |       |    4, 10    |
|       |  **4**  |        |       |    5, 7     |
|       |  **5**  |   6    |       |             |
|       |  **6**  |        |       |      9      |
|       |  **7**  |        |   8   |             |
|       |  **8**  |        |       |      9      |
|       |  **9**  |        |       |    4, 10    |
| **+** | **10**  |        |       |             |

#### (2) NFA $\to$ DFA (NFA转确定有限自动机)
展示子集构造法（Subset Construction）的结果。此阶段的状态列展示的是 NFA 状态的**集合**。

**示例数据与表现形式：**

| 标记  | 状态集合                  | letter                | digit                 |
| :---: | :------------------------ | :-------------------- | :-------------------- |
| **-** | **{ 1 }**                 | { 2, 3, 4, 5, 7, 10 } |                       |
| **+** | **{ 2, 3, 4, 5, 7, 10 }** | { 6, 9, 4, 7, 10, 5 } | { 8, 9, 4, 7, 5, 10 } |
| **+** | **{ 6, 9, 4, 7, 10, 5 }** | { 6, 9, 4, 7, 10, 5 } | { 8, 9, 4, 7, 5, 10 } |
| **+** | **{ 8, 9, 4, 7, 5, 10 }** | { 6, 9, 4, 7, 10, 5 } | { 8, 9, 4, 7, 5, 10 } |

#### (3) DFA $\to$ Min-DFA (DFA最小化)
展示最小化算法（如 Hopcroft 算法）处理后的最终 DFA。这是生成代码的基础。状态 ID 通常会被重新编号。

**示例数据与表现形式：**

| 标记  | 状态 ID | letter | digit |
| :---: | :-----: | :----: | :---: |
| **-** |  **1**  |   2    |       |
| **+** |  **2**  |   2    |   2   |

---

**编程提示：**
在实现界面时，表格控件（如 `QTableWidget` 或 `CListCtrl`）的**列数**是不固定的。你的程序必须解析正则表达式，提取出所有可能的输入符号（例如 `letter`, `digit`, `+`, `-` 等），然后动态创建表格的列头。

### 3.3 代码生成（核心难点）
*   **功能**：将**最小化DFA**转换为**C/C++词法分析源程序**。
*   **查看**：提供窗口查看生成的源代码。
*   **编译验证**：生成的源代码必须能被标准的C/C++编译器编译成可执行文件。
*   **生成方法约束**：**只能**采用`## 4. 词法分析程序生成方法规范`两种方法中的一种来生成代码。

### 3.4 GUI程序设计与交互要求

由于本系统涉及多个处理阶段（正则编辑、多步自动机转换、代码生成、测试验证），设计一个清晰、易用的图形用户界面（GUI）至关重要。

#### (1) 开发框架建议
*   **编程语言**：严格限制为 **C/C++**。
*   **GUI 库选择**：
    *   **推荐**：**Qt** (使用 C++ 开发，控件丰富，表格处理方便，跨平台)。
    *   **可选**：**MFC** (Microsoft Foundation Classes，经典Windows开发框架)。
    *   **可选**：**Win32 API** (底层开发，难度较大，不推荐用于此类复杂界面)。
    *   **其他**：wxWidgets, ImGui 等 C++ 图形库。

#### (2) 界面布局与功能区划分
系统主窗口建议采用 **多文档界面 (MDI)** 或 **标签页 (Tab Widget)** 结构，以容纳不同阶段的输出。界面应至少包含以下功能区域：

1.  **菜单栏/工具栏 (Menu/Toolbar)**：
    *   **文件操作**：新建、打开正则表达式文件、保存、退出。
    *   **执行操作**：一键转换（Run All）、分步转换（RE->NFA, NFA->DFA...）、生成代码、编译运行。
    *   **帮助**：关于软件、使用说明。

2.  **正则表达式编辑区 (Input Editor)**：
    *   **功能**：一个主要的多行文本编辑框（Text Edit），用于输入或显示从文件读取的正则表达式。
    *   **要求**：支持基本的文本编辑操作（复制、粘贴、撤销）。

3.  **自动机可视化区 (Visualization Area)**：
    *   **展示形式**：建议使用 **标签页 (Tabs)** 来切换展示不同的转换结果。
        *   Tab 1: **NFA状态表**
        *   Tab 2: **DFA状态表**
        *   Tab 3: **最小化DFA状态表**
    *   **表格控件要求**：
        *   **动态列**：由于正则表达式中的输入符号（Char set）是不确定的，表格控件必须能够根据分析结果**动态增加列**。
        *   **自适应**：行数随状态数量自动扩展。
        *   **只读**：用户不可修改生成的表格内容。

4.  **代码查看区 (Code Viewer)**：
    *   **功能**：独立窗口或标签页，显示生成的 C/C++ 词法分析器源代码。
    *   **交互**：提供“保存为 .cpp 文件”的按钮。

5.  **测试与验证区 (Testing Panel)**：
    *   这是一个综合交互区域，用于验证生成的分析器。
    *   **左侧/上方**：**源程序输入框**（加载 `sample.tny` 内容）。
    *   **操作按钮**：[词法分析/Run Lexer]。
    *   **右侧/下方**：**Token结果输出框**（显示 `sample.lex` 内容，即单词编码序列）。

#### (3) 交互逻辑流程
为了提升用户体验，建议设计如下的操作流：

1.  **启动**：程序启动，显示空白的正则表达式编辑区。
2.  **加载**：用户通过 `文件 -> 打开` 选择正则表达式文本文件，内容自动填充到编辑区。
3.  **转换**：用户点击 `转换` 按钮。
    *   系统后台执行 NFA -> DFA -> MinDFA 算法。
    *   界面下方的表格区域刷新，展示三个阶段的状态表。
4.  **生成**：用户点击 `生成代码` 按钮。
    *   系统弹出代码查看窗口或切换到代码标签页，展示生成的 `lex.cpp` 源码。
5.  **测试**：用户点击 `测试` 按钮或进入测试界面。
    *   用户加载 `sample.tny` 测试代码。
    *   点击运行，系统调用生成的分析器（或内部模拟执行），展示识别出的单词编码结果。

#### (4) 错误处理与提示
*   **状态栏**：在窗口底部显示当前操作状态（例如：“正在生成NFA...”、“转换成功”、“文件已保存”）。
*   **弹窗警告**：
    *   如果输入的正则表达式语法有误，应弹出错误提示框，说明错误原因。
    *   如果文件打开失败，应提示用户。

---

## 4. 词法分析程序生成方法规范

根据要求，生成的C/C++代码必须遵循以下两种结构之一：

### 【方法一】基于字符判断的直接控制流
该方法通过判断字符类型（`Judgechar`）来决定流程。

**生成的代码模板结构：**

```cpp
// 辅助函数：判断字符类型
int Judgechar(char ch) {
    if (isalpha(ch)) return 1;      // 是字母
    else if (isdigit(ch)) return 0; // 是数字
    return -1;                      // 其他
}

char GetNext() { return buffer[pos++]; }

// 核心分析函数
void GetToken() {
    if (Judgechar(ch) == 1) { // 状态1：检测到字母
        ch = GetNext();
        // 循环消耗后续的字母或数字
        while ((Judgechar(ch) == 1) || (Judgechar(ch) == 0)) {
            ch = GetNext();
        }
        cout << "ID"; // 识别结束，输出Token类型
    }
    else {
        cout << "Error";
    }
}
```

### 【方法二】基于 Switch-Case 的状态机
该方法显式地使用状态变量（`state`）在 `while` 循环中跳转。

**生成的代码模板结构：**

```cpp
int Judgechar(char ch) { /* 同上 */ }
char GetNext() { /* 同上 */ }

// 核心分析函数
void GetToken() {
    int state = 1;      // 初始状态
    bool done = false;  // 结束标志

    while (!done) {
        switch(state) {
            case 1: // 状态1逻辑
                if (Judgechar(ch) == 1) {
                    state = 2;       // 转移到状态2
                    ch = GetNext();
                } else {
                    cout << "Error!";
                    return;
                }
                break; // Switch结束

            case 2: // 状态2逻辑
                if ((Judgechar(ch) == 1) || (Judgechar(ch) == 0)) {
                    ch = GetNext();  // 保持在状态2
                } else {
                    done = true;     // 遇到非字母数字，结束
                }
                break;
        }
    }
    cout << "ID"; // 循环结束后输出Token
}
```

---

## 5. 输入文件定义规范 (Regex文件格式)

用户输入的正则表达式文件需遵循特定命名约定，以便系统识别哪些需要生成Token以及对应的编码。

1.  **宏定义**：不生成Token，仅作引用。
    *   格式：`name = expression`
    *   示例：`digit = [0-9]`
2.  **Token定义**：需生成DFA及代码。
    *   **命名开头**：必须以下划线 `_` 开头。
    *   **编码数值**：名字后紧跟数值，表示该单词的编码ID。
    *   **多词后缀**：若数值后跟 `S`，表示这是一组单词（如关键字或运算符），编码从该数值开始递增。
    *   示例：
        *   `_num100 = ...` (单词编码为100)
        *   `_ID101 = ...` (单词编码为101)
        *   `_special200S = ...` (如包含 `+ | - | *`，则 `+` 为200，`-` 为201，`*` 为202)

**示例输入文件内容：**
```text
digit = [0-9]
letter = [A-Za-z]
_num100 = (\+|-)?digit+
_ID101 = letter(letter|digit)*
_special200S = \+ | - | \* | / | = | < | <= | <<
```

---

## 6. 测试与验收流程

完成系统开发后，需使用 **TINY语言** 进行完整的测试流程，请参考附录中的**AI 自主测试**章节使用`Qt Test`对开发的程序进行对应的测试：

### 第一步：输入 TINY 语言定义
在你的系统中输入（或打开）包含TINY语言所有单词定义的正则表达式文件。

**TINY 语言单词定义参考：**
*   **关键字** (不区分大小写): `if`, `then`, `else`, `end`, `repeat`, `until`, `read`, `write`
*   **专用符号**: `+`, `-`, `*`, `/`, `%`, `^`, `<`, `<>`, `<=`, `>=`, `>`, `=`, `{`, `}`, `;`, `:=`
*   **ID (标识符)**: 字母开头，后跟任意字母或数字。
*   **Number (数字)**: 数字开头，后跟任意数字。
*   **注释**: 用 `{}` 包裹，可跨行，不可嵌套，不能出现在标记内部。
*   **空白**: 空格、换行、制表符。

### 第二步：生成与编译
1.  利用系统生成 TINY 语言的词法分析器源代码（C/C++）。
2.  使用外部 C/C++ 编译器（如 GCC, MSVC）编译该源代码，生成可执行文件（例如 `tiny_lexer.exe`）。

### 第三步：运行测试
1.  准备一个 TINY 语言的源代码文件 `sample.tny`。
2.  运行生成的 `tiny_lexer.exe`，读取 `sample.tny`。
3.  输出单词编码文件 `sample.lex`。
4.  **界面要求**：你的主系统需要提供窗口，分别查看 `sample.tny` (源文件) 和 `sample.lex` (结果文件) 的内容。

---

## 7. 提交内容清单
1.  **课程设计源程序**（你的生成器工具源码）。
3.  **系统使用说明书**（README）。
4.  **可执行程序**（你的生成器工具exe）。
5.  **测试相关**：TINY的正则表达式文本、`sample.tny`、生成的词法分析源码、编译出的扫描器、`sample.lex` 结果。
6.  **测试报告**。

## 8. AI 辅助开发与测试规范

本项目的开发采用 **AI 辅助编程** 模式。为了确保生成的代码健壮、可维护且易于通过 AI 进行调试，必须严格遵守以下开发、测试与日志规范。

### 8.1 开发环境与构建配置 (macOS + Qt)

*   **操作系统**：macOS
*   **开发框架**：Qt 6.x (C++)
*   **构建系统**：CMake + Ninja
*   **编译器**：
    - **macOS**: Clang (Xcode Tools)
    - **Windows**: MSVC 2019+ 或 MinGW 8.1+

**构建与运行命令参考：**

1.  **配置工程** (生成 Ninja 构建文件):
    ```bash
    # 确保 Qt 路径已在 PATH 中，或使用 qt-cmake
    qt-cmake -S . -B build-macos -G Ninja
    ```
2.  **编译源码**:
    ```bash
    cmake --build build-macos -j
    ```
3.  **运行应用**:
    *   **GUI 启动**: `open build-macos/byyl.app`
    *   **控制台启动** (推荐调试用): `build-macos/byyl.app/Contents/MacOS/byyl`
4.  **调试 (LLDB)**:
    ```bash
    lldb build-macos/byyl.app/Contents/MacOS/byyl
    ```
5.  **打包发布**:
    ```bash
    macdeployqt build-macos/byyl.app -verbose=2 -dmg
    ```

### 8.2 单元测试规范 (Unit Testing)

**原则**：核心算法逻辑必须与 GUI 解耦，并编写独立的单元测试。

*   **测试框架**：`QTest` (Qt Test Dictionary)
*   **覆盖范围**：
    *   **Regex 解析**：输入正则表达式，验证生成的后缀表达式或语法树结构是否正确。
    *   **自动机算法**：
        *   `Thompson算法`：输入简单正则，验证生成的 NFA 状态数和边。
        *   `子集构造法`：验证 $\epsilon$-closure 闭包计算是否正确。
        *   `最小化算法`：输入已知 DFA，验证分割后的分区是否正确。
    *   **代码生成器**：验证生成的 C++ 代码字符串是否包含预期的关键词。

**代码要求**：
所有核心算法类应位于独立的文件中（如 `RegexParser.cpp`, `Automata.cpp`），不包含任何 UI 代码（如 `QMessageBox`），以便在测试工程中独立链接。

### 8.3 GUI 集成测试规范 (UI Automation)

**原则**：使用代码模拟用户的鼠标点击、键盘输入，并验证界面反馈。

*   **测试工具**：`Qt Test` (需在 CMake 中引入 `Test` 组件)。
*   **控件命名要求**：
    *   所有关键交互控件（按钮、文本框、表格）必须在代码中设置 `objectName`。
    *   例如：`inputEditor->setObjectName("txtInputRegex");`，`btnConvert->setObjectName("btnStartConvert");`。
*   **测试流程模拟**：
    1.  **查找控件**：使用 `findChild<T>("name")` 获取控件指针。
    2.  **模拟操作**：使用 `QTest::keyClicks()` 模拟输入正则，使用 `QTest::mouseClick()` 模拟点击按钮。
    3.  **断言结果**：验证表格（QTableWidget）的行数、列头是否符合预期，或验证输出区文本是否非空。

**CMake 配置示例：**
```cmake
find_package(Qt6 REQUIRED COMPONENTS Widgets Test)
enable_testing()
add_executable(GuiTest tests/auto_test_ui.cpp)
target_link_libraries(GuiTest PRIVATE Qt6::Widgets Qt6::Test)
add_test(NAME GuiTest COMMAND GuiTest)
```

**测试代码示例：**
```cpp
void TestGui::testConversionFlow() {
    MainWindow window;
    window.show(); // 必须显示窗口以处理事件

    // 1. 获取控件
    auto edit = window.findChild<QTextEdit*>("txtInputRegex");
    auto btn = window.findChild<QPushButton*>("btnStartConvert");
    auto table = window.findChild<QTableWidget*>("tblNFA");

    QVERIFY(edit != nullptr);
    QVERIFY(btn != nullptr);

    // 2. 模拟输入 "a|b"
    QTest::keyClicks(edit, "a|b");

    // 3. 模拟点击转换
    QTest::mouseClick(btn, Qt::LeftButton);

    // 4. 验证结果 (假设 "a|b" 会产生 NFA 状态)
    QVERIFY(table->rowCount() > 0);
}
```

### 8.4 日志埋点与调试规范 (Logging)

为了便于 AI 分析运行错误，代码中必须包含结构化的日志输出。

*   **启用日志**：
    *   macOS 环境变量：`export QT_LOGGING_RULES="*.debug=true"`
*   **日志重定向**：
    *   必须使用 `qInstallMessageHandler` 将日志同时输出到**控制台**和**日志文件**（如 `app.log`）。
*   **埋点位置**：
    1.  **关键入口**：函数开始执行时，记录函数名及关键入参。
    2.  **分支决策**：在 `if/else` 或 `switch` 跳转时记录走到了哪个分支。
    3.  **异常捕获**：算法失败、文件读写失败时，必须输出 `qWarning` 或 `qCritical`。
    4.  **数据快照**：在自动机转换结束后，打印节点数量、边数量等摘要信息。

**日志格式示例**：
```cpp
// 推荐格式：[类别] [函数名] 消息内容
qDebug() << "[NFA_BUILD] buildThompson: input regex=" << regexStr;
qInfo()  << "[CONVERT] NFA to DFA finished. States:" << dfaStates.size();
qWarning() << "[FILE_IO] Failed to open file:" << fileName;
```

### 8.5 跨平台兼容性开发标准 (Critical)

为了保证 macOS 开发的代码在 Windows 上无需修改即可运行，必须严格遵守以下 API 使用规范：

1. **文件路径处理**：

   *   **禁止**使用硬编码的路径分隔符（如 `C:\\Users` 或 `/usr/bin`）。
   *   **必须**使用 `QDir::separator()` 或 `/`（Qt 内部会自动将 `/` 转换为 Windows 的 `\`）。
   *   **路径拼接**：使用 `QDir::filePath()` 或 `QFileInfo` 进行路径拼接。
   *   **标准路径**：获取桌面或文档目录时，**必须**使用 `QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)`。

2. **换行符与文本处理**：

   * **问题**：Windows 使用 `\r\n`，macOS/Linux 使用 `\n`。

   * **读取**：使用 `QTextStream` 读取文件，它会自动处理行尾符号标准化。

   * **正则解析**：在编写正则表达式解析器时，处理换行符应兼容 `\r` 和 `\n`，例如在跳过空白符时：

     ```cpp
     // 推荐
     if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') { skip(); }
     ```

3. **系统调用与打开文件**：

   *   **禁止**使用 `system("open ...")` (macOS Only) 或 `system("start ...")` (Windows Only)。
   *   **必须**使用 `QDesktopServices::openUrl(QUrl::fromLocalFile(path))` 来打开生成的文本文件或源代码文件。

4. **基本数据类型**：

   *   避免使用 `long`（在 Windows MSVC 下是 32位，在 macOS/Linux 64位下是 64位）。
   *   推荐使用定长类型 `int32_t`, `int64_t` 或 Qt 定义的 `qint32`, `qint64`。

5. **界面布局 (DPI Scaling)**：

   *   **禁止**使用绝对坐标（`move(100, 100)`）或固定像素大小（`setFixedSize`）。
   *   **必须**使用 **Qt Layouts** (`QHBoxLayout`, `QVBoxLayout`, `QGridLayout`) 进行布局，以适应 Windows 上常见的高 DPI 缩放设置（125%, 150%）。

6. **编码问题**：

   *   源文件必须保存为 **UTF-8 with BOM** 或 **UTF-8**。
   *   在代码中处理中文字符串时，统一使用 `QStringLiteral("中文")` 以避免 MSVC 编译器的编码识别错误。

---

# 附录：

## 1. TINY 语言的单词分类 (词法定义)

TINY 语言的词法规则如下，请以此为依据编写正则表达式文件。

### (1) 关键字 (Keywords)
以下单词为保留字，**不区分大小写**：
*   `if`
*   `then`
*   `else`
*   `end`
*   `repeat`
*   `until`
*   `read`
*   `write`

### (2) 专用符号 (Special Symbols)
包含算术运算符、比较运算符和分隔符：
*   `+`
*   `-`
*   `*`
*   `/`
*   `%`
*   `^`
*   `<`
*   `<>` (不等于)
*   `<=`
*   `>=`
*   `>`
*   `=`
*   `{`
*   `}`
*   `;`
*   `:=` (赋值)

### (3) 标识符 (Identifier) 与 数字 (Number)
*   **Identifier (标识符)**：以字母开头，后面可跟若干个字母或数字符号。**不区分大小写**。
*   **Number (数字)**：以数字符号开头，后面可跟若干个数字（仅支持整数）。

**正则表达式抽象定义：**
```text
letter = [a-zA-Z]
digit  = [0-9]
identifier = letter(letter|digit)*
number = digit+
```

### (4) 空白 (Whitespace)
空格由以下字符组成，用于分隔 Token：
*   空白符 (Space)
*   换行符 (Newline)
*   制表符 (Tab)

### (5) 注释 (Comments)
*   **格式**：使用花括号 `{ }` 包裹注释内容。
*   **位置**：可以放在任何允许出现空白的位置（即注释不能插入到关键字或标识符内部）。
*   **特性**：
    *   允许跨行。
    *   **不支持嵌套**（即 `{...{...}...}` 是非法的）。

---

## 2. TINY 语言的语法 (BNF 文法)

以下是 TINY 语言的上下文无关文法（Context-Free Grammar），采用 BNF (巴科斯范式) 表示。这将用于后续的语法分析测试。

```ebnf
program -> stmt-sequence

stmt-sequence -> stmt-sequence ; statement 
               | statement

statement -> if-stmt 
           | repeat-stmt 
           | assign-stmt 
           | read-stmt 
           | write-stmt

if-stmt -> if exp then stmt-sequence end 
         | if exp then stmt-sequence else stmt-sequence end

repeat-stmt -> repeat stmt-sequence until exp

assign-stmt -> identifier := exp

read-stmt -> read identifier

write-stmt -> write exp

exp -> simple-exp comparison-op simple-exp 
     | simple-exp

comparison-op -> < | > | = | <= | <> | >=

simple-exp -> simple-exp addop term 
            | term

addop -> + | -

term -> term mulop factor 
      | factor

mulop -> * | / | % | ^

factor -> ( exp ) 
        | number 
        | identifier
```

## 3. 项目相关

### 目录结构（简）
- 源码：`main.cpp`、`mainwindow.h/.cpp/.ui`
- 构建：建议使用独立目录，如 `build-macos/`（macOS）或 `build/`（Windows）

### 在 macOS 上构建与运行
- 配置（生成 Ninja 构建文件）：
  - `qt-cmake -S . -B build-macos -G Ninja`
- 编译：
  - `cmake --build build-macos -j`
- 运行图形应用：
  - `open build-macos/byyl.app`
- 直接运行二进制（便于捕获日志）：
  - `build-macos/byyl.app/Contents/MacOS/byyl`
- 调试（命令行）：
  - `lldb build-macos/byyl.app/Contents/MacOS/byyl`
- 打包（收集依赖并生成 dmg）：
  - `macdeployqt build-macos/byyl.app -verbose=2 -dmg`

提示：若 `cmake` 找不到 Qt，可改用 `qt-cmake`（如上），或者为普通 `cmake` 添加 `-DCMAKE_PREFIX_PATH=<你的 Qt 路径>`。

### 日志与调试建议
- 启用 Qt 日志：
  - `export QT_LOGGING_RULES="*.debug=true"`（macOS）
  - `set QT_LOGGING_RULES=*.debug=true`（Windows）
- 将日志重定向到文件（可在代码中使用 `qInstallMessageHandler`）。

### AI 自主测试
- 使用 `Qt Test` 在进程内模拟点击与输入，命令行执行并解析结果：
  - 在 CMake 中添加组件：`find_package(Qt6 REQUIRED COMPONENTS Widgets Test)`
  - 增加测试目标并注册：`enable_testing()`、`add_test(NAME GuiTest COMMAND GuiTest)`
  - 运行：`ctest --test-dir build-macos -V`
- 优点：跨平台、稳定，能直接断言控件状态与输出；适合与 CI/AI 测试集成。
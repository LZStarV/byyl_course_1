# byyl 词法分析生成器（Qt+CMake）

- 环境：macOS，Qt 6.9.3，CMake 4.2.0，Ninja 1.13.2
- 构建：`qt-cmake -S . -B build-macos -G Ninja`，`cmake --build build-macos -j`
- 运行：`open build-macos/byyl.app` 或 `build-macos/byyl.app/Contents/MacOS/byyl`
- 测试：`ctest --test-dir build-macos -V`

## 功能概览
- 正则解析：支持连接、`|`、`*`、`+`、`?`、`[]`、`()`、`\` 转义、命名宏 `name = expr`
- 自动机转换：RE→NFA（Thompson）、NFA→DFA（子集构造）、DFA→MinDFA（Hopcroft）
- 表格展示：动态列头（字母表与 `#` 表示 ε），首列标记（`-` 初态、`+` 终态）
- 代码生成：从 MinDFA 生成 C/C++ 源码（方法二：Switch-Case），字符串已格式化并带换行
- 测试与验证：并行扫描所有 Token 规则，最长匹配 + 权重策略，跳过空白与 `{...}` 注释

## 界面使用
- 页签与控件（objectName）：
  - 正则编辑：`txtInputRegex`，`btnLoadRegex`，`btnStartConvert`
  - NFA/DFA/MinDFA 表：`tblNFA`，`tblDFA`，`tblMinDFA`
  - 代码查看：`txtGeneratedCode`，`btnGenCode`
  - 测试与验证：左侧 `txtSourceTiny`（标签：源程序输入），右侧 `txtLexResult`（标签：Token 编码输出），`btnRunLexer`
- 操作步骤：
  - 在“正则编辑”页点击“从文件加载”，选择正则文件（示例：`tests/regex/javascript.regex`）
  - 点击“转换”查看三阶段状态表
  - 切到“测试与验证”，左侧输入源文本（JS/TINY 片段），点击“运行词法分析”查看右侧编码输出
  - 若左侧为空，系统会尝试加载 `resources/sample.tny`；仍为空则注入示例文本并提示状态栏
  - 若输出包含 `ERR`，状态栏会提示“存在未识别的词法单元(ERR)，请检查正则与输入”

## 命令行测试
- 构建与运行全部测试：
  - `cmake --build build-macos -j`
  - `ctest --test-dir build-macos -V`
- 测试目标：
  - `GuiTest`：UI 自动化，验证关键控件与基本流程
  - `CliRegexTest`：命令行管线（lex→parse→NFA→DFA→MinDFA→runMultiple），严格断言无 `ERR`
  - `CodegenTest`：生成器输出的 C++ 源码可编译、可运行
- JS 用例示例（已在测试覆盖）：
  - 输入：`abc123 456 def789` → 输出无 `ERR`
  - 输入：`if return == var abc123` → 输出无 `ERR`
  - 合并输入行：`abc123 def456\nif return == var abc123` → 输出无 `ERR`

## 代码生成
- 生成方法：Switch-Case 状态机（方法二）
- 生成内容包含：`Judgechar`、`AcceptState`、`Step` 等函数，格式化换行与缩进
- 在“代码查看”页点击“生成代码”可查看生成字符串；也可复制到本地进行独立编译

## 目录结构
- `byyl_course_design_1/`：主窗口与 UI（`mainwindow.h/.cpp/.ui`，`main.cpp`）
- `src/core/`：核心逻辑（与 GUI 解耦）
  - `regex/`：`RegexLexer.*`，`RegexParser.*`
  - `automata/`：`Thompson.*`，`SubsetConstruction.*`，`Hopcroft.*`
  - `generator/`：`CodeGenerator.*`
  - `model/`：`Alphabet.h`，`Automata.h`
  - `Engine.*`：统一编排与运行器（构建所有规则的 MinDFA 并并行扫描）
- `tests/`：`GuiTest`、`CliRegexTest`、`CodegenTest`
- `resources/`：示例 `tiny.regex` 与 `sample.tny`
- `CMakeLists.txt`：应用与测试目标定义

## 构建与运行
- 配置：`qt-cmake -S . -B build-macos -G Ninja`
- 编译：`cmake --build build-macos -j`
- 运行 GUI：`open build-macos/byyl.app` 或 `build-macos/byyl.app/Contents/MacOS/byyl`
- 运行测试：`ctest --test-dir build-macos -V`

## 常见问题
- 找不到 Qt：优先使用 `qt-cmake`，或为 `cmake` 添加 `-DCMAKE_PREFIX_PATH=$(brew --prefix qt)`
- 表格无内容：检查是否加载了有效的正则（至少包含一条以 `_` 开头的 Token 规则）
- 输出包含 `ERR`：检查关键字大小写、操作符转义（如 `\+`、`\*`、`\(`、`\)`），以及输入文本是否符合正则

## 开发规范
- 日志：建议启用 `export QT_LOGGING_RULES="*.debug=true"`，在关键路径输出结构化日志
- 跨平台：路径用 `QDir/QFileInfo`，文件用 `QTextStream`；兼容 `\n/\r`；布局使用 `QLayout`；源码 UTF-8
- UI 自动化：关键控件均设置 `objectName`，`Qt Test` 通过 `findChild` 与 `QTest` 进行交互与断言

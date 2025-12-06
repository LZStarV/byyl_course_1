Mini C 语义动作说明

一、文件格式
- 每条产生式用两行表示：第一行是产生式，第二行是对应的语义动作位序列。
- 多候选用竖线分隔：第一行 `A -> α1 | α2 | ...`，第二行对应写成 `bits(α1) | bits(α2) | ...`。
- 候选右部按空格切分为符号序列（包含终结符与非终结符，如 `(` `)` `[` `]` `,` `;` 等）。动作位序列中每个数字与右部同位置的符号一一对应。
- 空串使用 `#` 表示；空串候选的动作位允许写 0 或 1（用于给归约结果打标，不产生子节点）。

二、动作位含义
- 0：忽略该位置的符号，不把它加入当前规约产生的语义节点的孩子列表。
- 1：把该位置的符号作为孩子加入当前规约产生的语义节点。
- 2：把该位置的符号标记为“根候选”，用于设置当前规约产生的语义节点的标签。
- 备注：如果同一候选中出现多个 2，最终采用配置的根选择策略（默认 last_1）选取一个作为根标签来源；孩子顺序按右部出现顺序（默认 rhs_order）。

三、词素与叶子标签
- 对于被配置为“标识符”的 Token（如 `ID`），移进时叶子节点的标签默认替换为其词素文本；其它 Token 直接以 Token 名字作为标签。

四、示例（摘自 `tests/test_data/semantic/minic.txt`）
- `variable-definition -> type-indicator ID ; | type-indicator ID [ NUM ] ;`
  - 动作：`1 2 0 | 1 2 0 1 0 0`
  - 解释：前者取 `type-indicator` 为子、`ID` 为根、`;` 忽略；后者在数组定义中把 `NUM` 也作为子，其它括号与分号忽略。
- `function-definition -> type-indicator ID ( parameters ) compound-stmt`
  - 动作：`1 2 0 1 0 1`
  - 解释：以 `ID` 为根，类型、参数、函数体作为孩子，括号忽略。
- `compound-stmt -> { local-definitions statement-list }`
  - 动作：`0 1 1 0`
  - 解释：忽略花括号，仅保留两部分内容为孩子。
- `call -> ID ( arguments )`
  - 动作：`2 0 1 0`
  - 解释：以函数名 `ID` 为根，`arguments` 为孩子，括号忽略。
- `factor -> ( expression ) | variable | call | NUM | ++ variable | -- variable`
  - 动作：`0 1 0 | 1 | 1 | 1 | 1 2 | 1 2`
  - 解释：括号表达式只保留 `expression`；自增/自减以 `variable` 为根候选并作为孩子。

五、编写与校验规则
- 非空候选：动作位数必须等于右部符号数。
- 空候选 `#`：动作位数允许为 0 或 1。
- 建议把语法结构性符号（括号、逗号、分号）标为 0，避免产生无意义的叶子。
- 语义文件导入时将逐候选比对右部符号并校验位数；校验通过后用于 LR(1) 语义树构建。

六、Mini C BNF（参考）
- program -> definition-list
- definition-list -> definition-list definition | definition
- definition -> variable-definition | function-definition
- variable-definition -> type-indicator ID ; | type-indicator ID [ NUM ] ;
- type-indicator -> int | float | real | void
- function-definition -> type-indicator ID ( parameters ) compound-stmt
- parameters -> parameter-list | void
- parameter-list -> parameter-list, parameter | parameter
- parameter -> type-indicator ID | type-indicator ID [ ]
- compound-stmt -> { local-definitions statement-list }
- local-definitions -> local-definitions variable-definition | #
- statement-list -> statement-list statement | #
- statement -> expression-stmt | compound-stmt | condition-stmt | while-stmt | for-stmt | return-stmt
- expression-stmt -> expression ; | ;
- condition-stmt -> if ( expression ) statement | if ( expression ) statement else statement
- while-stmt -> while ( expression ) statement
- for-stmt -> for ( for-init ; expression ; for-update ) statement
- for-init -> expression | variable-definition | #
- for-update -> expression-list | #
- expression-list -> expression-list, expression | expression
- return-stmt -> return ; | return expression ;
- expression -> variable = expression | simple-expression
- variable -> ID | ID [ expression ]
- simple-expression -> additive-expression relop additive-expression | additive-expression
- relop -> <= | < | > | >= | == | !=
- additive-expression -> additive-expression addop term | term
- addop -> + | -
- term -> term mulop factor | factor
- mulop -> * | / | % | ^
- factor -> ( expression ) | variable | call | NUM | ++ variable | -- variable
- call -> ID ( arguments )
- arguments -> argument-list | #
- argument-list -> argument-list, expression | expression

七、常见问题
- “动作位数与候选符号数不匹配”：请检查右部符号是否逐个用空格分开；空候选仅允许 0 或 1 位。
- 识别 `ID` 的叶子显示词素：需要在运行 LR(1) 分析时提供与 `ID` 紧随的词素序列。

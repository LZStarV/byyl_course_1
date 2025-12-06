# 设置页面说明

## 目录与输出
- `generated_output_dir`：词法生成根目录（默认：`generated/lex`）。
- `syntax_output_dir`：语法生成目录（默认：`<generated_output_dir>/syntax`）。
- `graphs_dir`：图导出根目录（默认：`<generated_output_dir>/graphs`；语法图位于其 `syntax/` 子目录）。
- `config_search_paths`：配置文件搜索路径数组（分号分隔），用于覆盖默认查找路径。

## 权重与跳过
- `weight_tiers`：权重阈值数组（例：`[{"min_code":220,"weight":3},...]`）。
- 跳过项：`brace/line/block/hash/single/double/template` 对应注释与字符串的跳过开关。

## 词法与标识符
- `token_map.use_heuristics`：启发式映射开关（依据规则名与组生成直观的终结符名）。
- `whitespaces`：空白字符集合（支持 `\t`、`\n`、`\r`）。
- `emit_identifier_lexeme`：组合扫描时在 `identifier` 后追加词素（用于语义叶子标签）。
- `identifier_token_names`：视为标识符的规则名数组（大小写不敏感，按包含判断）。
- Token 头部解析配置：`token_header.prefix/name_first_ranges/name_rest_ranges/code_digit_ranges/group_suffix/group_suffix_optional`（默认兼容 `_NAME123` / `_NAME123S`）。

## 语法与算法
- `epsilon_symbol`、`eof_symbol`、`aug_suffix`：ε/EOF/增广后缀。
- `nonterminal_pattern`：非终结符匹配正则（可空）。
- 操作符集合：`grammar_tokens.multi_ops` 与 `grammar_tokens.single_ops`。
- `lr1_conflict_policy`：`prefer_shift|prefer_reduce|error`（全局冲突策略）。
- `lr1_prefer_shift_tokens`：优先移进的终结符列表（逗号分隔；如 `else,;`）。在 LR(1) 存在多动作时，若 lookahead 命中该列表且有移进项，则优先选移进；否则按全局策略选择。用于解决“悬挂 else”与“语句序列分号”两类典型冲突。

## 表头与 DOT
- 表头与 ε 列名：`i18n.table_mark/table_state_id/table_state_set/epsilon_column_label`。
- DOT 样式：`dot.rankdir/node_shape/epsilon_label`。

## Graphviz
- `graphviz.executable`：`dot` 可执行名（默认：`dot`）。
- `graphviz.default_dpi`：默认 DPI（默认：`150`）。
- `graphviz.timeout_ms`：渲染超时毫秒数（默认：`20000`）。

## 语义策略
- `semantic_actions.root_selection_policy`：根选择策略（如 `first_1/last_1`）。
- `semantic_actions.child_order_policy`：子节点顺序策略（如 `rhs_order`）。

## 保存与格式
- 设置页保存会写入 `config/lexer.json`（或自定义搜索路径中的第一个可写位置），仅输出显式配置项。
- 若某项为空或未修改，保持默认值或不写入；运行时会按默认与环境变量进行合并。

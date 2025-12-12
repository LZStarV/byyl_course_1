# 04 子集构造算法（NFA→DFA）

## 数据结构

表 1 子集构造中的关键结构与中间变量
对象或变量名称 功能 存储结构
DFAState / DFA 确定状态与自动机（id/nfaSet/accept/trans/start/alpha/states） 结构体
QSet<int> S/U 状态集合（当前/目标） 集合
QQueue<QSet<int>> work 工作队列（待处理集合） 队列
QMap<QString,int> idmap 集合名到状态 id 的映射 映射
QString setName(const QSet<int>&) 集合命名（稳定字符串化） 函数
Alphabet alpha 字母表（按 ordered() 遍历符号） 自定义类型
epsilonClosure(nfa,S) 计算 ε-闭包 子过程
move(nfa,S,a) 计算在符号 a 下的直接可达集合 子过程

## 算法实现过程

1. 初始集合与编号：以 NFA 起始态的 ε-闭包作为 `S`，用 `setName(S)` 命名；为其分配 `id=1` 并作为 `DFA.start`，将 `S` 入队到 `work`。
   - `startSet = {nfa.start}`，`st = epsilonClosure(nfa, startSet)`，`nameStart = setName(st)`；`idmap[nameStart] = 1`
   - 建立 `d0`：`d0.id=1`，`d0.nfaSet=st`，`d0.accept` 初始为 `false`
   - `work.enqueue(st)`；`nextId = 2`
2. 接受态判定：若 `S` 中任一 NFA 状态 `accept=true`，则将对应 `DFAState.accept=true`。
   - 具体做法：遍历 `st` 或新集合 `U` 中的每个 `s`，若 `nfa.states[s].accept` 为真，则将 `d0.accept` 或 `ds.accept` 置真
   - 该判定保证“包含接受态的集合”在确定化后依然是接受态
3. 广度推进（按字母表）：出队一个集合 `S`，遍历 `alpha.ordered()` 中每个符号 `a`。
   - 计算 `U = epsilonClosure(nfa, move(nfa, S, a))`：先以 `move` 求直接字符边可达集合，再以 ε-闭包扩张；若 `U` 为空则跳过，不记录迁移
   - 集合命名与编号：`Uname = setName(U)`；若 `idmap` 尚无该键，则 `uid = nextId`、登记映射、构造 `ds`（判定接受性、记录 `nfaSet=U`）、`d.states[uid]=ds`、`work.enqueue(U)`、`nextId++`
   - 复用已有编号：若 `Uname` 已存在，则 `uid = idmap[Uname]`
   - 记录当前集合到目标集合的确定转移：`d.states[sid].trans[a] = uid`
   - 遍历顺序由 `alpha.ordered()` 保证稳定输出（表格渲染时按字母表顺序显示）
4. 去重与稳定：依靠 `idmap` 保证集合唯一编号与去重，直到 `work` 队列耗尽。
   - `idmap` 的键由 `setName` 生成稳定字符串（集合按升序序列化），避免重复集合被赋予多个编号
   - 循环终止条件：当 `work` 空，表示所有新集合均已被处理且无新增集合入队
5. 输出与展示：结果 `DFA` 中的每个状态带有 `nfaSet` 展示（如 `{1,3,5}`），便于查阅与表格渲染。
   - `d.alpha = nfa.alpha`，确保字母表一致性；表格展示时可直接按符号列显示 `trans[a]`
   - `nfaSet` 字段用于直观展示该确定状态对应的 NFA 集合来源（便于调试与验证）

下图展示子集构造的核心循环（ε-闭包与 move 以及集合去重与编号）：
<!--【流程图】此处需要添加 '子集构造流程图'：闭包/move/编号的循环示意。 -->

## 各种情况讲解

- 无转移/死状态：当某符号在集合上无可达转移时，保持空或 `-1`，表中显示为空白。
- 字符类与单字符：字母表可能包含字符类标签，需在 `move` 中逐字符匹配或合并处理（与构造阶段一致）。
- 不可达集合：若新集合未能扩展转移，仍可保留编号但不会被后续访问。

## 关键代码片段（含注释）

```
// 子集构造入口：将 NFA 转为等价的 DFA
class SubsetConstruction { public: static DFA build(const NFA& nfa); };

// 用于展示 DFA 的状态表（符号列→目标集合/状态）
static Tables tableFromDFA(const DFA& dfa);
```

```
// 计算一个状态集合的 ε-闭包
static QSet<int> epsilonClosure(const NFA& nfa, const QSet<int>& S)
{
    QSet<int>   c = S;
    QQueue<int> q; for (int s : S) q.enqueue(s);
    while (!q.isEmpty())
    {
        int u = q.dequeue();
        auto it = nfa.states.find(u); if (it == nfa.states.end()) continue;
        for (auto e : it->edges)
        {
            if (e.epsilon && !c.contains(e.to)) { c.insert(e.to); q.enqueue(e.to); }
        }
    }
    return c;
}

// 计算一个状态集合在符号 a 下的直接转移集合
static QSet<int> move(const NFA& nfa, const QSet<int>& S, const QString& a)
{
    QSet<int> r;
    for (int u : S)
    {
        auto it = nfa.states.find(u); if (it == nfa.states.end()) continue;
        for (auto e : it->edges) { if (!e.epsilon && e.symbol == a) r.insert(e.to); }
    }
    return r;
}

// 子集构造主过程：从 NFA 构建等价的 DFA
DFA SubsetConstruction::build(const NFA& nfa)
{
    DFA d; d.alpha = nfa.alpha;
    int nextId = 1; QMap<QString, int> idmap; QQueue<QSet<int>> work;
    // 初始集合：起始态的 ε-闭包
    QSet<int> startSet; startSet.insert(nfa.start);
    auto st = epsilonClosure(nfa, startSet); auto nameStart = setName(st);
    idmap[nameStart] = nextId;
    // 建立初始 DFA 状态并判定接受态
    DFAState d0; d0.id = nextId; d0.nfaSet = st; d0.accept = false;
    for (int s : st) if (nfa.states[s].accept) d0.accept = true;
    d.states.insert(nextId, d0); d.start = nextId; work.enqueue(st); nextId++;
    // 广度遍历状态集合，按符号构造转移
    while (!work.isEmpty())
    {
        auto S = work.dequeue(); auto Sname = setName(S); int sid = idmap[Sname];
        for (auto a : d.alpha.ordered())
        {
            auto U = epsilonClosure(nfa, move(nfa, S, a)); if (U.isEmpty()) continue;
            auto Uname = setName(U); int uid;
            // 新集合赋新编号并加入队列；否则复用已有编号
            if (!idmap.contains(Uname))
            {
                uid = nextId; idmap[Uname] = uid;
                DFAState ds; ds.id = uid; ds.nfaSet = U; ds.accept = false;
                for (int s : U) if (nfa.states[s].accept) ds.accept = true;
                d.states.insert(uid, ds); work.enqueue(U); nextId++;
            }
            else uid = idmap[Uname];
            d.states[sid].trans[a] = uid;
        }
    }
    return d;
}
```

## 输入/输出与示例

输入：NFA 的状态与转移（概念化表示）。
输出：DFA 的状态转移表（符号→目标集合/状态）。

下图为“子集构造流程图”占位：
<!--【流程图】此处需要添加 '子集构造流程图'：ε-闭包/move/去重编号/转移构造。 -->

## 单元测试

输入：
```
_tok100 = a | b
```

预测结果：
- 通过子集构造得到的 DFA 不包含任何 ε 转移；
- 至少一个确定状态出现，且符号转移为显式字符。

测试结果：
- 结构检查通过：所有转移符号均为常规字符，无 ε；
- GUI 截图：自动机核心单元测试（FACoreTest）DFA 部分的运行截图（含转移检查输出）。
<!--【GUI 截图】此处需要添加 'FACoreTest DFA 运行截图'：显示 DFA 无 ε 转移的检查结果。 -->

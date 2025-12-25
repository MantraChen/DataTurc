FUNCTION maximumScore(grid) :
    n ← grid 的行数
    IF n == 1 :
    RETURN 0

    ─────────────────────────────
    // 1. 预处理列前缀和
    // sm[col][row] = 第 col 列从 1 到 row 的元素和
    ─────────────────────────────
    INITIALIZE sm[1..n + 1][0..n] = 0

    FOR col = 1 TO n :
FOR row = 1 TO n :
sm[col][row] ← sm[col][row - 1] + grid[row - 1][col - 1]

─────────────────────────────
// 2. DP 定义
// f[i][j][0/1]:
//   已处理到第 i 行
//   当前选择在第 j 列
//   状态 0 / 1 表示不同结构状态
─────────────────────────────
INF ← 极大值
INITIALIZE f[0..n][0..n][0..1] = -INF

─────────────────────────────
// 3. 状态转移
─────────────────────────────
FOR i = 1 TO n :
FOR j = 1 TO n :

// P4：单独起一个状态
f[i][j][1] ← sm[i - 1][j] + sm[i + 1][j]

// ───── P1.1：从 i-1 行，左侧转移（保持状态 1）
IF i > 1:
FOR jj = 1 TO j :
det ←(sm[i - 1][j] - sm[i - 1][jj])
+ sm[i + 1][j]
- sm[i][jj]

f[i][j][1] ← max(
    f[i][j][1],
    f[i - 1][jj][1] + det
)

// ───── P1.2：从 i-1 行，右侧转移（到状态 0）
IF i > 1:
FOR jj = j + 1 TO n :
det ← sm[i + 1][j] - sm[i][j]

f[i][j][0] ← max(
    f[i][j][0],
    f[i - 1][jj][0] + det,
    f[i - 1][jj][1] + det
)

// ───── P2.1：从 i-2 行，左侧转移（到状态 1）
IF i > 2:
FOR jj = 1 TO j :
det ←(sm[i - 1][j] - sm[i - 1][jj])
+ sm[i + 1][j]

f[i][j][1] ← max(
    f[i][j][1],
    f[i - 2][jj][0] + det,
    f[i - 2][jj][1] + det
)

// ───── P2：从 i-2 行，右侧转移（到状态 1）
IF i > 2:
FOR jj = j + 1 TO n :
det ← sm[i + 1][j]

f[i][j][1] ← max(
    f[i][j][1],
    f[i - 2][jj][0] + det,
    f[i - 2][jj][1] + det
)

// ───── P3：从 i-3 行，全列转移（到状态 1）
IF i > 3:
FOR jj = 1 TO n :
det ← sm[i - 1][j] + sm[i + 1][j]

f[i][j][1] ← max(
    f[i][j][1],
    f[i - 3][jj][0] + det,
    f[i - 3][jj][1] + det
)

─────────────────────────────
// 4. 取全局最大值
─────────────────────────────
ans ← 0
FOR i = 1 TO n :
FOR j = 1 TO n :
ans ← max(ans, f[i][j][0], f[i][j][1])

RETURN ans

---
title: Title
separator: --s--
verticalSeparator: --v--
theme: simple
highlightTheme: tomorrow-night-bright
revealOptions:
  width: 1520
  height: 950
  margin: 0.04
  transition: 'convex'
  slideNumber: true
---

# Recover the Design
杨淦  汤宇帆  潘阔

--s--

## 问题分析
--v--
### 1. 题目描述
在一个 $n \times m$ 的矩阵中，每个单元格 $(i, j)$ 有一个数字 $D_{i,j} \in \{0, 1, 2, 3, 4\}$。

该数字表示该点需要连出的线段数量（即**度数**或**接口数**）。
--v--
**目标：** 找到一种连线方式，使得：
* 所有点的连线数量（实际度数）满足其给定的数字 $D_{i,j}$。
* 任意两条连线不能交叉。
* 连线只能沿着网格线进行，并且连线必须连接到另一个度数大于 0 的节点。

--s--

## 算法与数据结构
--v--
### 1. 数据结构
--v--
**数据结构：** 使用三维数组 `map[51][51][6]` 存储信息（基于 $n, m < 50$）：
* `map[x][y][0]`：该点的**初始度数**（即 $D_{x,y}$）。
* `map[x][y][1]` 到 `map[x][y][4]`：分别存储**上、右、下、左**方向的连线状态（`1` 表示已连接，`0` 表示未连接）。
* `map[x][y][5]`：该点当前**剩余可连接接口数**。
* **路径标记：** 空白单元格（$D_{x,y}=0$）在连线经过后会被标记为 `map[x][y][0] = -1`，表示该路径已被占用，后续连线不能经过。
--v--
### 2. 算法思路
--v--
**第一阶段：确定性优化 (optimize.h)**
* 采用**贪心策略**和**剪枝思想**，遍历所有节点，寻找可以**唯一确定**连线关系的节点。
* **确定条件：** 如果一个节点的**剩余接口数** `map[i][j][5]` 等于其**可连接的邻居节点数** `available_connections`，则该节点必须与所有可连接的邻居建立连线。
* 不断循环此过程，直到一轮循环中没有任何新的连线被确定，以最大化预处理。
--v--
**第二阶段：回溯搜索 (backtracking.h)**
* 对经过第一阶段优化后**仍有剩余接口**的节点，进行**深度优先搜索 (DFS) 回溯**。
* 按照预先确定的节点顺序 `list`（所有 $D_{i,j} > 0$ 的节点），逐个处理。
* 对于当前节点，尝试所有可能的未连接方向和目标节点组合。
--v--
**核心剪枝/限制：**
* **连线终点检查：** `find_up/down/left/right` 函数中：
    * 必须连接到 `map[i][j][0] > 0` 的非空节点。
    * 目标节点的剩余接口数必须大于 0 (`map[i][j][5] > 0`)。
* **路径交叉剪枝：** 在连线路径上：
    * 不能经过另一个非空节点 (`map[i][j][0] > 0`，但不是目标节点)。
    * 不能经过已被占用的空白路径节点 (`map[i][j][0] == -1`)。

--s--

## 具体代码实现

### 1. 优化阶段 (`optimize.h`)
--v--
**`check_direction_connection` 函数：**
```cpp [88-90|93-102]
 int check_direction_connection(int i, int j, int dir, int* target_i, int* target_j, int n, int m) {
    int step = 1;
    
    while (1) {
        int current_i = i, current_j = j;
        
        // 计算当前检查的坐标
        switch(dir) {
            case 1: current_i = i - step; break; // 上
            case 2: current_j = j + step; break; // 右
            
            case 3: current_i = i + step; break; // 下
            case 4: current_j = j - step; break; // 左
        }
        
        // 检查边界 (坐标从1开始，所以边界是1到n和1到m)
        if (current_i < 1 || current_i > n || current_j < 1 || current_j > m) {
            return 0; // 超出边界，无法连接
        }
        
        // 检查是否遇到非空节点
        if (map[current_i][current_j][0] > 0) {
            // 找到非空节点，检查是否可连接
            if (map[current_i][current_j][5] > 0) {
                *target_i = current_i;
                *target_j = current_j;
                return 1; // 找到可连接节点
            } else {
                return 0; // 遇到已连接满的节点，无法连接
            }
        }
        
        // 检查是否遇到已占用的空节点
        if (map[current_i][current_j][0] == -1) {
            return 0; // 遇到已占用的空节点，无法连接
        }
        
        step++;
    }
}
```
* 用于检查从 $(i, j)$ 向给定方向 `dir` 延伸，是否能找到一个**可连接**（`>0` 且 `[5]>0`）的节点。
* 路径上遇到 `map[i][j][0] == -1` (即边界) 或已连满的节点则返回 `0`。
--v--
**`optimize` 函数：**
```cpp [129-136|141-145|151-166]
void optimize(int n, int m) {
    int changed;
    do {
        changed = 0;
        
        // 循环从1开始到n和m
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                // 只处理非空节点
                if (map[i][j][0] <= 0) continue;
                
                // 计算当前剩余连接数 = 初始度数 - 已连接方向数
                int connected_count = 0;
                for (int dir = 1; dir <= 4; dir++) {
                    if (map[i][j][dir] == 1) {
                        connected_count++;
                    }
                }
                int remaining_connections = map[i][j][0] - connected_count;
                
                // 如果剩余连接数为0，跳过此节点
                if (remaining_connections <= 0) continue;
                
                // 检查四个方向，统计可连接的节点数量
                int available_connections = 0;
                int target_i[4] = {0};
                int target_j[4] = {0};
                int dirs[4] = {0};
                
                for (int dir = 1; dir <= 4; dir++) {
                    // 如果这个方向已经连接，跳过
                    if (map[i][j][dir] == 1) continue;
                    
                    // 检查这个方向是否可以连接
                    int ti, tj;
                    if (check_direction_connection(i, j, dir, &ti, &tj, n, m)) {
                        target_i[available_connections] = ti;
                        target_j[available_connections] = tj;
                        dirs[available_connections] = dir;
                        available_connections++;
                    }
                }
                
                // 如果可连接节点数量等于剩余连接数，建立所有连接
                if (available_connections == remaining_connections && available_connections > 0) {
                    for (int k = 0; k < available_connections; k++) {
                        int dir = dirs[k];
                        int ti = target_i[k];
                        int tj = target_j[k];
                        
                        // 标记起点节点的连接方向
                        map[i][j][dir] = 1;
                        
                        // 标记终点节点的反向连接
                        int reverse_dir;
                        switch(dir) {
                            case 1: reverse_dir = 3; break; // 上 -> 下
                            case 2: reverse_dir = 4; break; // 右 -> 左
                            case 3: reverse_dir = 1; break; // 下 -> 上
                            case 4: reverse_dir = 2; break; // 左 -> 右
                        }
                        map[ti][tj][reverse_dir] = 1;
                        
                        // 标记路径上的空节点
                        int step = 1;
                        while (1) {
                            int current_i = i, current_j = j;
                            
                            // 计算当前路径节点坐标
                            switch(dir) {
                                case 1: current_i = i - step; break;
                                case 2: current_j = j + step; break;
                                case 3: current_i = i + step; break;
                                case 4: current_j = j - step; break;
                            }
                            
                            // 如果到达终点节点，停止
                            if (current_i == ti && current_j == tj) break;
                            
                            // 标记路径上的空节点
                            if (map[current_i][current_j][0] == 0) {
                                map[current_i][current_j][0] = -1; // 标记为已占用
                                
                                // 标记路径节点的双向连接
                                if (dir == 1 || dir == 3) { // 垂直方向
                                    map[current_i][current_j][1] = 1; // 上
                                    map[current_i][current_j][3] = 1; // 下
                                } else { // 水平方向
                                    map[current_i][current_j][2] = 1; // 右
                                    map[current_i][current_j][4] = 1; // 左
                                }
                            }
                            
                            step++;
                        }
                        
                        // 更新终点节点的剩余连接数
                        map[ti][tj][5]--;
                        
                        changed = 1;
                        
                        printf("Connected (%d,%d) to (%d,%d) with direction %d\n", 
                               i, j, ti, tj, dir);
                    }
                    
                    // 更新当前节点的剩余连接数
                    map[i][j][5] = map[i][j][0];
                    for (int dir = 1; dir <= 4; dir++) {
                        if (map[i][j][dir] == 1) {
                            map[i][j][5]--;
                        }
                    }
                }
            }
        }
        
    } while (changed);
}
```
* 使用 `do-while(changed)` 循环，确保反复遍历直到无节点可确定。
* 对于每个节点，计算 `remaining_connections` 和 `available_connections`。
* 若 `remaining_connections == available_connections`，则执行连线操作：
    * 更新起点和终点的连接状态 (`map[...][dir/reverse_dir] = 1`)。
    * 更新路径上空节点的占用状态 (`map[...][0] = -1`) 和连接标记。
    * 更新起点和终点的剩余接口数 (`map[...][5]--`)。
--v--
### 2. 回溯阶段 (`backtracking.h`)
--v--
**`find_up/down/left/right` 函数：**
* 功能与 `check_direction_connection` 类似，但更进一步：
    * **不仅检查可行性，还同时标记路径上的空节点** (`map[...][0] = -1`)，实现剪枝。
    * 如果找到目标节点，返回其坐标；如果遇到障碍或边界，返回 `0`。
--v--
**`clear_up/down/left/right` 函数：**
* **回溯的关键操作：** 清除由 `find_` 函数在当前路径上设置的 `-1` 标记及其连接状态，恢复现场，以便进行下一个方向的试探。
--v--
**`back_tracking` 函数：**
* **基线条件：** 当 `index == c_size` 时，表示所有有接口的节点都已处理完毕，设置 `trigger = 1` 成功返回。
* **递归逻辑：** * 根据当前节点 $(x, y)$ 的**初始类型** (`type = map[x][y][0]`) 和**剩余接口数** (`rem = map[x][y][5]`)，确定需要尝试的连线组合。
    * 例如，`type=1` 时，尝试**上、右、下、左**四个方向的单边连接（1种组合）。
    * `type=2, rem=2` 时，尝试**上-右, 上-下, ...** 等所有六种双边连接（2种组合）。
    * **试探 -> 递归 -> 回溯：** 对于每种成功的连线尝试：
        1.  调用 `find_` 标记路径。
        2.  更新起点和终点的连接状态和剩余接口数。
        3.  递归调用 `back_tracking(index+1)`。
        4.  如果递归成功 (`trigger == 1`) 则立即返回。
        5.  **回溯：** 恢复连接状态和剩余接口数。
        6.  调用 `clear_` 清除路径标记。

--s--

## 复杂度分析

### 1. 优化阶段复杂度
--v--
* `optimize` 循环次数（外层 `do-while`）：最坏情况下，每轮只确定一条线，循环次数与线段总数相关，上限为 $O(N^2)$ 或 $O(NM)$。
* 内层循环：遍历所有节点 $N \times M$。
* `check_direction_connection`：在网格中直线搜索，最坏情况下遍历 $O(N+M)$ 个单元格。
* **总复杂度：** 难以精确计算，但由于每次成功优化都会减少问题规模，实际运行中效率较高。上界粗略估计为 $O((N \times M)^2 \times (N+M))$，但实际表现远优于此。

--v--

### 2. 回溯阶段复杂度
--v--
* **最坏情况：** 这是一个典型的 NP 完全问题（类似于图的哈密顿路径或约束满足问题）。
* 没有成功的优化预处理，回溯树的规模可能非常庞大。
* **节点顺序：** 按照 $D_{i,j} > 0$ 的节点列表 `list` 顺序进行回溯，顺序对性能有影响。
* **单次操作：** 每次连线尝试中，`find_` 和 `clear_` 函数需要 $O(N+M)$ 的时间复杂度。
* **剪枝的作用：** * 第一阶段的**确定性优化**可以大幅减少回溯节点的数量和剩余接口数。
    * 回溯过程中的**路径占用标记**（`-1`）和**接口耗尽检查** (`[5]==0`) 提供了强力的剪枝，避免了大量的无效搜索。
    * 整体而言，算法是**指数级别**的，但在约束强、规模小的（$N, M \le 50$）问题上可以接受。

--s--
## 运行测试
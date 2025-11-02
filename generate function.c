#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> // 引入 bool 类型

int main() {
    int n, m;
    // 新增参数：擦除直通节点（度为2且为直线）的概率百分比 (0-100)
    int erasure_percentage; 

    while(1) {
        // 1. 从标准输入读取 n, m 和擦除概率百分比
        if (scanf("%d %d %d", &n, &m, &erasure_percentage) != 3) {
            // 尝试读取 n, m 的终止输入
            if (n == 0 && m == 0) {
                break;
            }
            fprintf(stderr, "输入错误，请输入三个整数 n, m 和擦除概率百分比 (0-100)\n");
            return 1;
        }

        // Check for exit condition (如果在读取三个参数后，发现 n, m 都是 0)
        if (n == 0 && m == 0) {
            break;
        }

        // 检查约束
        if (n <= 1 || n > 50 || m <= 1 || m > 50) {
            fprintf(stderr, "n 和 m 必须大于 1 且小于等于 50\n");
            continue;
        }
        if (erasure_percentage < 0 || erasure_percentage > 100) {
            fprintf(stderr, "擦除概率百分比必须在 0 到 100 之间\n");
            continue;
        }

        srand(time(NULL));

        // 1. 创建内部的 "解决方案" 布局 (保持 50% 基础边连接概率)
        int max_n = (n > 50) ? 51 : n; // 动态数组大小
        int max_m = (m > 50) ? 51 : m; // 动态数组大小
        
        bool horizontal[max_n][max_m - 1];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m - 1; j++) {
                horizontal[i][j] = (rand() % 2 == 1);
            }
        }

        bool vertical[max_n - 1][max_m];
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < m; j++) {
                vertical[i][j] = (rand() % 2 == 1);
            }
        }

        // 2. 准备输出的 "题目" 矩阵
        int problem_grid[max_n][max_m];

        // 3. 遍历所有节点，计算度数并决定是否 "擦除"
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                
                bool has_left = (j > 0) && horizontal[i][j - 1];
                bool has_right = (j < m - 1) && horizontal[i][j];
                bool has_up = (i > 0) && vertical[i - 1][j];
                bool has_down = (i < n - 1) && vertical[i][j];

                int degree = (int)has_left + (int)has_right + (int)has_up + (int)has_down;

                // 检查是否为 "直通" 节点 (度数为2，且是水平或垂直直线)
                bool is_horizontal_passthrough = (degree == 2 && has_left && has_right && !has_up && !has_down);
                bool is_vertical_passthrough = (degree == 2 && !has_left && !has_right && has_up && has_down);

                if (is_horizontal_passthrough || is_vertical_passthrough) {
                    // 这是一个 "直通" 节点
                    
                    // 使用输入的百分比控制擦除概率
                    // rand() % 100 产生 0-99 的随机数，如果小于 erasure_percentage 则擦除
                    if (rand() % 100 < erasure_percentage) { 
                        problem_grid[i][j] = 0; // 擦除 (密度降低)
                    } else {
                        problem_grid[i][j] = 2; // 保留为连接器 (密度保持)
                    }
                } else {
                    // 非直通节点 (度数 0, 1, 3, 4, 或 2度拐角) 必须保留
                    problem_grid[i][j] = degree;
                }
            }
        }

        // 4. 打印输出
        printf("%d %d\n", n, m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                printf("%d", problem_grid[i][j]);
                if (j < m - 1) {
                    printf(" ");
                }
            }
            printf("\n");
        }
    }

    return 0;
}
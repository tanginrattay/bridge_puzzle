extern int map[51][51][6];

// 检查某个方向是否可以连接，并返回可连接的节点坐标
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
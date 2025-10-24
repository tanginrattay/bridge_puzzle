extern int map[50][50][6];

void find_determined_by_path_search(int n, int m) {
    int changed;
    
    do {
        changed = 0;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                int current_degree = map[i][j][0];
                
                // 只处理有值的节点
                if (current_degree <= 0) continue;
                
                // 计算已连接的数量
                int connected_count = 0;
                for (int dir = 1; dir <= 4; dir++) {
                    if (map[i][j][dir] == 1) {
                        connected_count++;
                    }
                }
                
                // 计算剩余需要连接的数量
                int remaining_connections = current_degree - connected_count;
                
                // 如果剩余连接数为0，跳过此节点
                if (remaining_connections <= 0) continue;
                
                // 检查四个方向
                for (int dir = 1; dir <= 4; dir++) {
                    // 如果这个方向已经确定连接状态，跳过
                    if (map[i][j][dir] != 0) continue;
                    
                    int ni = i, nj = j;
                    switch(dir) {
                        case 1: ni = i - 1; break; // UP
                        case 2: ni = i + 1; break; // DOWN
                        case 3: nj = j - 1; break; // LEFT
                        case 4: nj = j + 1; break; // RIGHT
                    }
                    
                    // 检查边界和是否有连接器
                    if (ni >= 0 && ni < n && nj >= 0 && nj < m && map[ni][nj][0] > 0) {
                        int neighbor_degree = map[ni][nj][0];
                        
                        // 计算邻居的已连接数量和剩余连接数
                        int neighbor_connected_count = 0;
                        for (int ndir = 1; ndir <= 4; ndir++) {
                            if (map[ni][nj][ndir] == 1) {
                                neighbor_connected_count++;
                            }
                        }
                        int neighbor_remaining = neighbor_degree - neighbor_connected_count;
                        // 如果剩余连接数相等且都大于0，建立连接
                        if (remaining_connections == neighbor_remaining && remaining_connections > 0) {
                            // 标记这个连接
                            map[i][j][dir] = 1;
                            int reverse_dir = (dir % 2 == 1) ? dir + 1 : dir - 1;
                            map[ni][nj][reverse_dir] = 1;
                            
                            changed = 1;
                            // 更新剩余连接数
                            remaining_connections--;
                            // 如果当前节点剩余连接数为0，跳出内层循环
                            if (remaining_connections == 0) {
                                break;
                            }
                        }
                    }
                }
            }
        }
        
    } while (changed);
}
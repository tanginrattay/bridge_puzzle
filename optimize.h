extern int map[51][51][6];

// Check if a direction can connect and return the coordinates of the connectable node
int check_direction_connection(int i, int j, int dir, int* target_i, int* target_j, int n, int m) {
    int step = 1;
    
    while (1) {
        int current_i = i, current_j = j;
        
        // Compute the coordinates currently being checked
        switch(dir) {
            case 1: current_i = i - step; break; // up
            case 2: current_j = j + step; break; // right
            
            case 3: current_i = i + step; break; // down
            case 4: current_j = j - step; break; // left
        }
        
        // Check bounds (coordinates start from 1, so bounds are 1..n and 1..m)
        if (current_i < 1 || current_i > n || current_j < 1 || current_j > m) {
            return 0; // Out of bounds, cannot connect
        }
        
        // Check if encountering a non-empty node
        if (map[current_i][current_j][0] > 0) {
            // Found a non-empty node, check if it's connectable
            if (map[current_i][current_j][5] > 0) {
                *target_i = current_i;
                *target_j = current_j;
                return 1; // Found a connectable node
            } else {
                return 0; // Encountered a fully connected node, cannot connect
            }
        }
        
        // Check if encountering an occupied empty node
        if (map[current_i][current_j][0] == -1) {
            return 0; // Encountered an occupied empty node, cannot connect
        }
        
        step++;
    }
}

void optimize(int n, int m) {
    int changed;
    do {
        changed = 0;
        
        // Loops from 1 to n and 1 to m
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                // Only process non-empty nodes
                if (map[i][j][0] <= 0) continue;
                
                // Compute remaining connections = initial degree - number of connected directions
                int connected_count = 0;
                for (int dir = 1; dir <= 4; dir++) {
                    if (map[i][j][dir] == 1) {
                        connected_count++;
                    }
                }
                int remaining_connections = map[i][j][0] - connected_count;
                
                // If remaining connections is 0, skip this node
                if (remaining_connections <= 0) continue;
                
                // Check four directions and count connectable nodes
                int available_connections = 0;
                int target_i[4] = {0};
                int target_j[4] = {0};
                int dirs[4] = {0};
                
                for (int dir = 1; dir <= 4; dir++) {
                    // If this direction is already connected, skip
                    if (map[i][j][dir] == 1) continue;
                    
                    // Check whether this direction can connect
                    int ti, tj;
                    if (check_direction_connection(i, j, dir, &ti, &tj, n, m)) {
                        target_i[available_connections] = ti;
                        target_j[available_connections] = tj;
                        dirs[available_connections] = dir;
                        available_connections++;
                    }
                }
                
                // If the number of available connections equals remaining connections, establish all connections
                if (available_connections == remaining_connections && available_connections > 0) {
                    for (int k = 0; k < available_connections; k++) {
                        int dir = dirs[k];
                        int ti = target_i[k];
                        int tj = target_j[k];
                        
                        // Mark the starting node's connection direction
                        map[i][j][dir] = 1;
                        
                        // Mark the endpoint node's reverse connection
                        int reverse_dir;
                        switch(dir) {
                            case 1: reverse_dir = 3; break; // up -> down
                            case 2: reverse_dir = 4; break; // right -> left
                            case 3: reverse_dir = 1; break; // down -> up
                            case 4: reverse_dir = 2; break; // left -> right
                        }
                        map[ti][tj][reverse_dir] = 1;
                        
                        // Mark empty nodes on the path
                        int step = 1;
                        while (1) {
                            int current_i = i, current_j = j;
                            
                            // Compute current path node coordinates
                            switch(dir) {
                                case 1: current_i = i - step; break;
                                case 2: current_j = j + step; break;
                                case 3: current_i = i + step; break;
                                case 4: current_j = j - step; break;
                            }
                            
                            // If reached the endpoint node, stop
                            if (current_i == ti && current_j == tj) break;
                            
                            // Mark empty nodes along the path
                            if (map[current_i][current_j][0] == 0) {
                                map[current_i][current_j][0] = -1; // mark as occupied
                                
                                // Mark the path node's bidirectional connections
                                if (dir == 1 || dir == 3) { // vertical direction
                                    map[current_i][current_j][1] = 1; // up
                                    map[current_i][current_j][3] = 1; // down
                                } else { // horizontal direction
                                    map[current_i][current_j][2] = 1; // right
                                    map[current_i][current_j][4] = 1; // left
                                }
                            }
                            
                            step++;
                        }
                        
                        // Update endpoint node's remaining connection count
                        map[ti][tj][5]--;
                        
                        changed = 1;
                        
                        /* printf("Connected (%d,%d) to (%d,%d) with direction %d\n", 
                               i, j, ti, tj, dir); */
                    }
                    
                    // Update current node's remaining connection count
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
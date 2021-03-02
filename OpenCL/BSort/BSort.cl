__kernel void BSort(__global int* data, uint stage_numbers, uint direction, __local int* local_data) {
    
    //initializing data for allocation memory
    uint local_id     = get_local_id(0);
    uint dir          = direction;
    uint group_size   = get_local_size(0);
    uint offset       = get_group_id(0) * group_size;

    //copied memory to the work group using pairs (from global memory)
    local_data[local_id] = data[offset * 2 + local_id];
    local_data[local_id + group_size] = data[offset * 2 + local_id + group_size];
    
    //waiting for all threads -> copyied full array
    barrier(CLK_LOCAL_MEM_FENCE);

    //standart sort : check and replace if necessary
    for(uint stage = 0; stage < stage_numbers; ++stage) {
        for(uint stage_pass = 0; stage_pass < stage + 1; ++stage_pass) {
            uint compare_distance = 1 << (stage - stage_pass);

            uint left_id = (local_id % compare_distance) + (local_id / compare_distance) * 2 * compare_distance;
            uint right_id = left_id + compare_distance;
    
            int left_elem = local_data[left_id];
            int right_elem = local_data[right_id];
    
            if((local_id / (1 << stage)) % 2 == 1)
                dir = 1 - dir;

            int greater = right_elem;
            int lesser = left_elem;
            if(left_elem > right_elem) {
                greater = left_elem;
                lesser  = right_elem;
            }
    
            //cheking for direction of comparison
            if(dir) {
                local_data[left_id]  = lesser;
                local_data[right_id] = greater;
            } else {
                local_data[left_id]  = greater;
                local_data[right_id] = lesser;
            }

            dir = direction;

            // waiting for sorting every pair
            barrier(CLK_LOCAL_MEM_FENCE);
        }
    }

    //copied memory back -> from local to global memory
    data[offset * 2 + local_id] = local_data[local_id];
    data[offset * 2 + local_id + group_size] = local_data[local_id + group_size];
}

__kernel void BSort_origin(__global int * data, uint stage, uint stage_pass, uint direction)
{
    uint id = get_global_id(0);
    
    uint compare_distance = 1 << (stage - stage_pass);

    uint left_id = (id % compare_distance) + (id / compare_distance) * 2 * compare_distance;
    uint right_id = left_id + compare_distance;
    
    int left_elem = data[left_id];
    int right_elem = data[right_id];
    
    if((id / (1 << stage)) % 2 == 1)
        direction = 1 - direction;

    int greater = right_elem;
    int lesser = left_elem;
    if(left_elem > right_elem) {
        greater = left_elem;
        lesser  = right_elem;
    }
    
    if(direction) {
        data[left_id]  = lesser;
        data[right_id] = greater;
    } else {
        data[left_id]  = greater;
        data[right_id] = lesser;
    }
}
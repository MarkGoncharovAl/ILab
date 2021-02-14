__kernel void BSort(__global int* data, uint stage_numbers, uint direction) {
    uint id           = get_global_id(0);
    uint dir          = direction;

    for(uint stage = 0; stage < stage_numbers; ++stage) {
        for(uint stage_pass = 0; stage_pass < stage + 1; ++stage_pass) {
            uint compare_distance = 1 << (stage - stage_pass);

            uint left_id = (id % compare_distance) + (id / compare_distance) * 2 * compare_distance;
            uint right_id = left_id + compare_distance;
    
            int left_elem = data[left_id];
            int right_elem = data[right_id];
    
            if((id / (1 << stage)) % 2 == 1)
                dir = 1 - dir;

            int greater = right_elem;
            int lesser = left_elem;
            if(left_elem > right_elem) {
                greater = left_elem;
                lesser  = right_elem;
            }
    
            if(dir) {
                data[left_id]  = lesser;
                data[right_id] = greater;
            } else {
                data[left_id]  = greater;
                data[right_id] = lesser;
            }

            dir = direction;
            barrier(CLK_LOCAL_MEM_FENCE);
            barrier(CLK_GLOBAL_MEM_FENCE);
        }
    }
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
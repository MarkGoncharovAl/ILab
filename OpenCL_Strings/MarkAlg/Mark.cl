__kernel void PrepareHashOfBuffer(__global char* buffer, __global ulong* hashes, char size) {
    
    uint id = get_global_id(0);
    
    //REALIZE
    char* cur_pointer = buffer + id;
    ulong hash = *cur_pointer;
    
    for (char i = 1; i < size; ++i)
    {
        cur_pointer++;
        hash = (hash << size) + *cur_pointer;
    }

    hashes[id] = hash;
}
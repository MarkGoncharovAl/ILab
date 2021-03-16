__kernel void PrepareHashOfBuffer(__global char* buffer, __global ulong* hashes, ulong size) {
    
    uint id = get_global_id(0);
    
    //REALIZE
    ulong hash = 1;
    char* str = buffer + id;

    for (ulong i = 0; i < size; ++i)
    {
        hash = ((hash << 4) + *str) % (1 << 31);
        str++;
    }

    hashes[id] = hash;
}
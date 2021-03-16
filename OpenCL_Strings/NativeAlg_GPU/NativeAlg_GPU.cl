__kernel void CheckHashes(

__global char* base, 
ulong size_base,
__global char* patterns, 
__global ulong* nums, 
__global ulong* out      )

{
    uint id = get_global_id(0);
    out[id] = 0;
    
    ulong start = 0;
    if (id != 0)
        start = nums[id - 1];
    
    ulong size = nums[id] - start; 

    char* pattern = patterns + start;
    char* last_base = base + size_base - size + 1;

    for (char* cur_elem = base; cur_elem < last_base; ++cur_elem)
    {
        ulong i = 0; 
        for (; i < size; ++i)
        {
            if (cur_elem[i] != pattern[i])
                break;
        }
        if (i == size)
            (out[id])++;
    }
    
}
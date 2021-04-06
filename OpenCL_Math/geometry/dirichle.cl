__kernel void Matrix (
    __global float* buffer,
    __global float* vector,
    __global float* result
)
{
    uint id = get_global_id (0);
    float cur_buffer = buffer[id + 1];
    if (cur_buffer == 0)
        return;

    uint shift = (uint)((float)id + buffer[0]);
    result[shift] += cur_buffer * vector[id];
}

__kernel void VectNum (
    float number, 
    __global float* vector
)
{
    uint id = get_global_id (0);
    vector[id] *= number;
}

__kernel void Sum (
    __global float* lhs,
    __global float* rhs
)
{
    uint id = get_global_id (0);
    lhs[id] += rhs[id];
}

__kernel void Sub (
    __global float* lhs,
    __global float* rhs
)
{
    uint id = get_global_id (0);
    lhs[id] -= rhs[id];
}
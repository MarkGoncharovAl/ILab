__kernel void Multiply (
    __global float* buffer,
    __global float* vector,
    __global float* result
)
{
    uint id = get_global_id (0);
    result[id] += buffer[id] * vector[id];
}

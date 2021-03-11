__kernel void RabKar(__global char* buffer, __global long int* hashes, uint size) {
    
    uint id = get_global_id(0);
    
    //REALIZE
    hashes[id] = get_hash(buffer + id, size);
}
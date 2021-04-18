# Bitonic Sorting - SYCL #
### Author: Goncharov Mark (EverRest) ###

### Содержание: ###
1. Task
2. Algorithm
3. Using

Task
====
##### Level 1: Fast sorting

Input: ```size``` of vector. Then waiting ```size``` elements.

Output: sorting array 

The main target is to accelerate sorting on GPU and be faster than std::sort. We used this type of sorting in order to get more parallel power.

Using
=====
* ```./main``` calles the main task
* ```./tests``` calles checking time of working on GPU and CPU

Algorithm
===========

1. Preparing buffer for sorting and local memory for part of full data
2. Sorting in groups that have size no more than (*)
3. Sorting in global memory remaining big parts 
4. Getting data form GPU-buffer
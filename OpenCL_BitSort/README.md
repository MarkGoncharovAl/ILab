# Bitonic Sorting #
### Author: Goncharov Mark (EverRest) ###

### Содержание: ###
1. Task
2. Algorithm
3. Installing
4. Using

Task
====
##### Level 1: Fast sorting

Input: ```size``` of vector. Then waiting ```size``` elements.

Output: sorting array 

The main target is to accelerate sorting on GPU and be faster than std::sort. We used this type of sorting in order to get more parallel power.

Algorithm
===========

1. Preparing buffer for sorting and local memory for part of full data
2. Sorting in groups that have size no more than (*)
3. Sorting in global memory remaining big parts 
4. Getting data form GPU-buffer

(*) CL_DEVICE_MAX_WORK_GROUP_SIZE

Installing
=========
* Clonning this repository
>git clone https://github.com/MarkGoncharovAl/ILab
* Install OpenCL
>sudo apt install opencl-c-headers opencl-clhpp-headers
* For testing you must have Google Tests
>sudo apt install libgtest-dev
* Cmake is required
>sudo apt install cmake
* Creating installing folder

>mkdir build

>cd build

* Building a project 
>cmake ../ -DMAIN=ON -DTEST=ON -DE2E=ON
>make

Using
=====
1. ```./main``` calles default first finding GPU device
2. ```./main --help ``` calles help for choosing devices
3. ```./main --device <num>``` calles programm with chosen number of device

#### February 2021
# SYCL #
### Author: Goncharov Mark (EverRest) ###

### Содержание: ###
1. Task
2. Installing

Task
====
##### Level 4: Rewritting to SYCL

Until now we've been working with OpenCL and outer-compiled kernels. For C++ program it's no more than txt document. 
But the most interesting case is to build kernel into .cpp, for getting outer variables for example and so on

Typical function on SYCL that [adds 2 vectors on GPU](https://github.com/codeplaysoftware/computecpp-sdk/blob/master/samples/simple-vector-add.cpp)

The task of this level is to rewrite 1-3 levels into SYCL. [Other documentation](https://sycl.tech/).

Installing
=========
* Must be installed packet [Logging](https://github.com/MarkGoncharovAl/ILab/tree/master/Errors) <- click go to the link
* Must be installed [oneAPI DPC++](https://intel.github.io/llvm-docs/GetStartedGuide.html)
* Make an attetion to pathes of ```clang++``` and ```ld_library_path```. Without them cmake can't find compilers
    1. export PATH=$DPCPP_HOME/llvm/build/bin:$PATH
    2. export LD_LIBRARY_PATH=$DPCPP_HOME/llvm/build/lib:$LD_LIBRARY_PATH

#### March 2021
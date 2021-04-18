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

Typical function on SYCL: 
> https://github.com/codeplaysoftware/computecpp-sdk/blob/master/samples/simple-vector-add.cpp
It summarize 2 vectors on GPU

The task of this level is to rewrite 1-3 levels into SYCL. Other documentation in sycl.tech

Installing
=========
* Must be installed oneAPI DPC++:
> https://intel.github.io/llvm-docs/GetStartedGuide.html
* Make an attetion to pathes of clang++ and ld_library_path. Without them cmake can't find compilers
> export PATH=$DPCPP_HOME/llvm/build/bin:$PATH
> export LD_LIBRARY_PATH=$DPCPP_HOME/llvm/build/lib:$LD_LIBRARY_PATH

In my case only ```set PATH /home/mark/TRASH/DPC/llvm/build/bin/ /usr/local/sbin /usr/local/bin /usr/sbin /usr/bin /sbin /bin /usr/games /usr/local/games```,
because I work using fish-shell.

#### March 2021
export PATH=/home/mark/TRASH/DPC/llvm/build/bin:$PATH
export LD_LIBRARY_PATH=/home/mark/TRASH/DPC/llvm/build/lib
clang++ -fsycl ../main.cpp ../MySycl/MySycl.cpp -o main
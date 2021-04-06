# Using of OpenCL - Dirichle's equation #
### Author: Goncharov Mark (EverRest) ###

### Содержание: ###
1. Task
2. Algorithm
3. NVidia code
4. Installing
5. Using

Task
====
##### Level 3: Dirichle's problem
Formally, there is an easy way to solve using multiplications of matrixes.
We will optimise this work in GPU

Theory:
https://developer.nvidia.com/gpugems/gpugems2/part-vi-simulation-and-numerical-algorithms/chapter-44-gpu-framework-solving

Input you can see in folder ```TESTS```

Our main target is to solve inner task Dirichle for Lapla's equation.

Algorithm
===========

1. Preparing matrix and free coefficients
2. Solving in using CPU (to check results)
3. Use Conjugate gradient method: https://en.wikipedia.org/wiki/Conjugate_gradient_method
4. During process, we're using fast multiplications of matrixes

NVidia code
===========
 Code from the article for easy reading:
> Example 44-1. The Conjugate Gradient Solver
> ```c
> void clCGSolver::solveInit() 
> {    
>   Matrix->matrixVectorOp(CL_SUB, X, B, R); 
>   // R = A * x - b    
>   R->multiply(-1);  
>   // R = -R    
>   R->clone(P);  
>   // P = R    
>   R->reduceAdd(R, Rho);  
>   // rho = sum(R * R);    
> }  
> 
> void clCGSolver::solveIteration() 
> {    
>   Matrix->matrixVectorOp(CL_NULL, P, NULL,Q);  
>   // Q = Ap;    
>   P->reduceAdd(Q, Temp);  
>   // temp = sum(P * Q);    
>    Rho->div(Temp, Alpha);  
>   // alpha = rho/temp;      
>   X->addVector(P, X, 1, Alpha);  
>   // X = X + alpha * P    
>   R->subtractVector(Q, R, 1, Alpha);  
>   // R = R - alpha * Q    
>   R->reduceAdd(R, NewRho);  
>   // newrho = sum(R * R);    
>   NewRho->divZ(Rho, Beta);  
>   // beta = newrho/rho      
>   R->addVector(P, P, 1, Beta);  
>   // P = R + beta * P;    
>   clFloat *temp; temp=NewRho;    
>   NewRho=Rho; Rho=temp;  
>   // swap rho and newrho pointers  
> }  
> 
> void clCGSolver::solve(int maxI) 
> {    
>   solveInit();    
>   for (int i = 0; i < maxI; i++) 
>       solveIteration();  
> }      
> 
> int clCGSolver::solve(float rhoTresh, int maxI) 
> {    
>   solveInit(); Rho->clone(NewRho);    
>   for (int i = 0; i < maxI && NewRho.getData() > rhoTresh; i++)       
>       solveIteration();    return i;  
> } 
> ```
> 
> The new values for y can be computed easily with our framework by applying one matrix-vector operation:
> ````c
> clMatrix->matrixVectorOp(CL_SUB, clCurrent, clLast, clNext);    
> clLast->copyVector(clCurrent);        
> // save for next iteration  
> clCurrent->copyVector(clNext);        
> // save for next iteration  
> cluNext->unpack(clNext);              
> // unpack for rendering    
> renderHF(cluNext->m_pVectorTexture);  
> // render as height field 
> ````
> 
> The program to implicitly solve the wave equation now looks like this
> ````c
> cluRHS->computeRHS(cluLast, cluCurrent); 
> // generate c(i, j, t)  
> clRHS->repack(cluRHS);                   
> // encode into RGBA    
> iSteps = pCGSolver->solve(iMaxSteps);    
> // solve using CG    
> cluLast->copyVector(cluCurrent);         
> // save for next iteration    
> clNext->unpack(cluCurrent);              
> // unpack for rendering  
> renderHF(cluCurrent->m_pVectorTexture); 
> ```` 

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
>cmake ../ -DMAIN=ON -DGENERATE=ON -DCOMPARE=ON
>make

Using
=====
1. ```./main``` calles default first finding GPU device
2. ```./main --help ``` calles help for choosing devices
3. ```./main --device <num>``` calles programm with chosen number of device

#### March 2021
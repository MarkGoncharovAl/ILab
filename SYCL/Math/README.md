# Using of OpenCL - Dirichle's equation #
### Author: Goncharov Mark (EverRest) ###

### Содержание: ###
1. Task
2. Using
3. Algorithm
4. NVidia code

Task
====
##### Level 3: Dirichle's problem
Formally, there is an easy way to solve using multiplications of matrixes.
We will optimise this work in GPU

Theory:
https://developer.nvidia.com/gpugems/gpugems2/part-vi-simulation-and-numerical-algorithms/chapter-44-gpu-framework-solving

Input you can see in folder ```TESTS```

Our main target is to solve inner task Dirichle for Lapla's equation.

Using
=====
```./main``` calles default first finding GPU device

Algorithm
===========

1. Preparing matrix and free coefficients
2. Solving in using CPU (to check results)
3. Use Conjugate gradient method: https://en.wikipedia.org/wiki/Conjugate_gradient_method
4. During process, we're using fast multiplications of matrixes

NVidia code
===========
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
>   NewRho=Rho; 
>   Rho=temp;  
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
>       solveIteration();    
>   return i;  
> } 
> ``` 

#### March 2021
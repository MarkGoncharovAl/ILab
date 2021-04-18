# Using of OpenCL - Findings patterns #
### Author: Goncharov Mark (EverRest) ###

### Содержание: ###
1. Task
2. Using
3. Algorithm

Task
====
##### Level 2: Pattern matching

1. Input is a long string (format: size, then the string).
2. Then waiting for several strings (format: count of strings. For each string must be size and string)

```cpp
//Example:
11 abracadabra
3
3 rac
5 barak
2 ab
```

3. Output is a count of findings for each substring (numeration is started from 1)

```cpp
1 1
2 0
3 2
```

The main target is to accelerate searching on GPU and be faster than CPU. In several articles (like below) authors maintain that they can win about 30 times!

```https://developer.nvidia.com/gpugems/gpugems3/part-v-physics-simulation/chapter-35-fast-virus-signature-matching-gpu```

Using
=====
1. ```./main``` calles default first finding GPU device
2. ```./tests``` calles checking tests. It can set some time - wait!

Algorithm
===========

1. Preparing hashes tables in GPU
2. Copying final data to hash table of std::vector`s
3. Cheking hash of pattern in CPU
4. If hashes are equal, we`re cheking for pure hit

#### March 2021
# Logging in C++ #
## Author: MarkGoncharovAl ##

### Description ###
1. Compiling
2. Flags
3. Functions

### Install ###
1. Clone this folder
2. Using gcc compile: 
```gcc "files.c" Errors/Errors.cpp -o "output.o"```
3. Using CMake: after compiling "output.o" paste:
	* ```add_subdirectory(Errors)```
	* ```target_link_libraries("output.o" Errors)```

### Flags ###
The simplest understanding is looking at Examples! 
There're few convenient features that you can use

##### Compiling flags: #####

* ```ERROR``` - priting fatal + errors

Using of this (and others) flags:
```gcc "files.c"  Errors/Errors.cpp -o "output.o" -D ERROR```

* ```WARNING``` - fatal + errors + warnings
* ```DEBUG``` - fatal + errors + warnings + debug
* ```TRACE``` - fatal + errors + warnings + debug + trace 
 
``` LOG_FILES ``` differ from others:

As default debbuging file is "debug.log". You can change it (next chapter).
But in any case file "debug.log" will be created.
In order to avoid it use this flag.

BUT: you have to set directory at the beggining of the programm in this case!

### Functions ###
You can use it in namespace LOG

1. ```LOG::ChangeDirectory``` - reset debuging directory. Previous will be closed.
2. ```LOG::SetSize_NameFile``` - reset size of string that used for FILE name.
3. ```LOG::SetSize_NameLine``` - reset size of string that used for LINE name.

### I hope this will help you with debugging! ###
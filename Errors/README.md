# Logging in C++ #
## Author: MarkGoncharovAl ##

If you want to use logging functional without installing -> copy ```Logging``` folder 
to your project folder. Then apply ```add_subdirectory(Logging)``` and ```target_link_libraries(Logging_lib)```

### Description ###
1. Install
2. Using
3. Flags
4. Functions

### Install ###
1. Clone this folder
2. Write ```make``` in order to install packet on your computer

### Using ###
1. Use in your program as others libraries
	* ```#include <Logging>```
2. Compiling using terminal. Example: ```g++ main.cpp -lLogging```
	* ```[Your compiler] [Your files.cpp] -lLogging```

3. Or compile using CMake:
	* ```find_package (Logging CONFIG REQUIRED)```
	* ```target_link_libraries (${Final Binary} PRIVATE ${Your other libraries} Logging)```

### Flags ###
The simplest understanding is looking at Examples! 
There're few convenient features that you can use:

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

#### March 2021

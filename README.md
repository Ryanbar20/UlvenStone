# UlvenStone
## Description
A Wolfenstein/DOOM style renderer in C using SDL <br>
It does not contain any parts of the original Wolfenstein/DOOM assets
## Requirements
You will need to install SDL2 on your system before being able to run this program.<br>
You will also need to put an SDL2.dll file such that main.c in src can access it <br>
E.g. put this file in the src folder <br>
You will need to have make installed on your system to compile the program

## Compilation
### Renderer
To compile the renderer, you enter a terminal in Ulvenstone/src and run the following command:
```shell
    make main
```
### Tester
To compile the testing program, you enter a terminal in Ulvenstone/test and run the following command:
```shell
    make testUtils
```

## Execution
### Renderer
After compiling, you can run the renderer by running the src\ulvenstone executable
### Tester
After compiling, you can run the testing program by running the test\testprogram executable

## LICENSE
This software uses SDL2, find its license in license/SDL.txt<br>
This program is licensed under MIT, find more info in the licence/LICENSE.txt file
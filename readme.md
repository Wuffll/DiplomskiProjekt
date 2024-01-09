## Master's project

## Requirements

- OpenGL 4.5
- MS Visual Studio 2022

## Troubleshooting problems
There are several things to keep in mind when the program isn't able to execute or throws an exception.

1. Check whether there is a message in the command line. I tried to print out a message before throwing an exception to see what might be the issue.
2. If there is a message about a missing .dll file, make sure to put the .dll file from the Dependencies/dll folder into the folder where the executable is. **As of writing this**, that only includes the file for Assimp (assimp-vc143-mt.dll).
3. Make sure that the *#define*'s at the beginning of the main.cpp file are correct according to your repository directory. If you are running this on your machine, you will most likely (99.9%) have to change the paths in the main.cpp file.
---

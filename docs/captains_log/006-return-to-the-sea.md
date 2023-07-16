Stardate - 4/16/22

I have chosen to write my compiler in C++, as this will most likely require a low-level touch.
It has, however, been quite a while since I have written any serious C++ and I am starting to remember why I disliked trying to compile on Windows as well as my deep-seated hatred of Visual Studio. I have tried multiple (free/open source) IDEs, but have had issues with most of them and, as much as I hate to admit it VS Code has been the best to use. I am also using this opportunity to learn about CMake, which is a daunting task, but a useful one.

As for Sigil, my struggles with building and *include paths* have made me consider how exactly I want the operation of the compiler to work. I dislike the command line tools for building c/c++ and java, particularily the necessity to specify every file necessary for building. This is especially cumbersome in C/C++ as it leads to very flat folder structures (typically only a source and include folder) which I find annoying when your codebase/number of files becomes sufficiently large.

I want Sigil to be structured similarly to python, with packages and modules.
Package == folder
Module == file

The module/file name becomes the namespace.
Due to intricacies of python, depending on where you run a file from, the interpreter may or may not be able to find dependecies.
This is particularily prevalent when trying to run a "main" of a nested file that has dependecies in packages above it. This is usually solved by just
changing what your working directory is, but is an annoyance nonetheless.
This is something I want to avoid with Sigil.

Since I have chosen ".sg" as my file extension, I am naming my compiler "sgc".
I want sgc to act something like to this in the simplest case

sgc --source [root source dir] --main [path to main file] --out [executable name]

This way, all you have to specify is the root of your source tree and leave the rest to the compiler.
Adding a "main" argument allows for multiple insertion points to be defined. Similar to java, but this is defined at compile time.
I may look into a way to have more than one insertion point, but that may be a bit difficult.

sgc flow

1) Check for main file
  1.1) Parse main file and check for main method?
  1.2) Add imports to queue
    1.2.1) Add imported objects to set of items that need to be located
2) Loop over import queue until set of unkown imported names is empty
3) Drop all objects/funcs that are not called upon by main, i.e. things that will never occur during the course of the program


But first, I need to work on the parser. Now there are existing parser-generators (YACC, Bison, LEXX, etc) but I am here for the challenge so I am going to write my own. I'd like it to generate syntax trees using files that contain an EBNF description of Sigil as an input. 

IDEA: later, for efficiency, look into having a compiler for the ebnf into a serializable format that doesn't need to be parsed or error checked.
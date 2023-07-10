Herein lies my thoughts on the languages that I am using as references.

Primarily this will be C++, Java, and Python but I may branch out with thoughts on other languages that I have used in the past, or steal ideas from other languages.


### C++
This is the big one. The one every knows (and fears). C++ is the father of all modern Object-Oriented-Programming. As such it is also... old. C++ is still widely used and remains the goto for anything requiring a precise touch and getting as low-level as possible without going to assembly.

Things I don't like about C++:
* Header files
* Memory leaks
* cout

Things I like about C++:
* Explicit call by value or by ref/pointer
* SPEEEED
* Strongly typed
* public/private access
* operator overloading
* typedefs
* namespaces

### Java
I have had a good amount of experience with Java. Primarily through my college studies since it was the language of choice for a lot of my classes as it was by far the easiest for the professor's to test with minimal issue. This is a good example of one of Java's key selling points: portability. Java is a bit of a mutant language, being both compiled and interpreted. But, it allows easy portability between operating systems and CPU architectures without needing to distribute the source or build it yourself.

Now, as for the language itself, it is very C-like. The first thing one might notice is that all Java code must be contained in a class definition. Even the main function(s) must be contained in a class. I can only assume they chose to do this as it provides a mechanism similar to C++ namespaces, but I would also say that this was already handled via the handy-dandy Java package system. As such, I find it a bit cumbersome when defining entirely static functions. Java also has the requirement that code filenames must be the same as the class it is defining. This has to do with the package system and how java finds the definitions via the filename. One thing I like about Java is its package system. Packages are 1) explicitly specified in the file itself, and 2) organized into a directory structure.

Java also has the unusual feature of allowing multiple main functions. Every class you define can have it's own main, but you can still only run one at a time (although I guess you could call main manually from another class). When running a Java program, you specify the classname as the program name and that defines which main is run. This is an interesting feature (similar to how you can specify any file as "main" in python), but I am unsure as to how useful it actually is.


Things I like about Java:
* Automatic garbage collection, although it can contribute to slower code as this is handled by the Java interpreter
* Reference semantics, although I like having the option of the amount of control a C++ pointer has
* explicit package names and little argument when trying to access code parent packages
	Also running Java programs in different folders doesn't change the package lookup path
* PORTABILITY
* JAR files. Really just zip files, but you can put whatever you want and access it at runtime. Useful for embedding things (like, for instance SQL queries) as separate files without needing them as strings in your code


Things I don't like about Java:
* Lack of function pointers
	These kinda exist now in the latest versions via lambda interfaces, but they seem hacky
* Lack of default func arguments
	The canonical way to handle this is to use function overloading, but I find it leads to less readable code.
* The primitive type wrapper classes and needing to use them in class templates. Also those wrappers have wierd properties that can't be replicated in client code, i.e. automatic conversion to and from primitive types. 
* Having to specify public and private for every function.
* extreme verbosity in typenames, Java is very wordy
* Java Developers... builder-functions, lambdas, futures

### Python
Over the past year, I have written a lot of Python (maybve too much). Despite how much power a very high-level lang gives you, I still find it to be lacking in some ways. In particular, I dislike it for much the same reasons that I find JavaScript to be a terrible language: weakly-typed variables. While an argument can be made that typeless variables and arguments improve code readability and reduces typing (the kind you do with your fingers), in the end I find it harms the developer more-so when they cannot easily discern the type of an object without manually checking it (which in the nd adds more code). I find this especially annoying when using 3rd party libraries and they either:
  1) Don't use type hints
  2) did complicated things with packages and whatnot, breaking any IDE's ability to find the types
  3) Poorly document their API and expected argument types

No to mention that IDE's will struggle with any static error checking and also have a tendancy to tell me an argument is the wrong type or even that it doesn't exist, but then the code still runs as expected. This is why I pretty much always use type hints in my code, it helps a lot, but I feel like it is a janky and forced system because of how unenforced it is.

A list of other things I dislike about Python
* lack of true public/private class members
* lack of true abstract classes/interfaces
* lack of true enums
* lack of true const values
* manually specifying "self" for EVERY class variable and method declaration and usage
* lack of true multithreading
* the whole no curly brackets thing is meh and causes more issues when autoformatting if nothing else
* why is True and False capitalized?
* lack of true multi-line comments, like... why?
* "elif" ...need I say more
* occassional difficulty with the import system (this is mitigated by running your code as a module, but still, meh)
* *underscores* ughhhhh

Despite this, there are still quite a lot of things I DO like about python
* simplicity in creating new projects/package structure
* list comprehensions (obviously... who doesn't abuse these?)
* simple operator/keyword overloading (although I don't necessarily like the dunderscore naming scheme)
* keyword only arguments
* functions as first-class objects
* lists, dicts, and sets as standard language constructs
* the freakin print func is always available and not "System.out.println()" or ""cout << nonsense"
* F strings
* array slicing
* ternary if-else using keywords
* __init__.py allowing setup code when importing a package, but really I find this to be bad practice
Stardate - 4/22/22

While I was beginning to write out some early descriptions of Sigil's grammer, I had a fantastic idea to have my compiler be able to read in some EBNF rules and use that to parse the language.
Now, there are programs out there that already do this, so called "Compiler Compilers" (Lex, YACC, and Bison to name a few) that can be used to create simple languages in and of themselves, but can also be manipulated into spitting out syntax trees and the like. But, of course, it would be too constricting to try and use one of these for Sigil and, frankly, too easy.

Therefor, I have created my own version of EBNF, PEEBNF! The "Poorly-Extended Extended Backus-Naur Form." This is so far basically the same as EBNF, although without the fancy things like alternation and repetition, which may come into play at a later date, but most likely will just be implemented with some form of preprocessing.

So, while implementing said EBNF parser in C++, I quickly realized that this was something I could do very easily using Python's regex module. This then lead down the rabbit hole of me trying to learn how to embed python into C, which isn't too bad in itself, but I had a hell of a time getting it to compile. 
This was mainly due to the fact that:
1) I was building C++ on Windows which is always annoying
2) I was using the MingW compiler and the Python .lib files were compiled for MSVC

But of course, I was using CMake, so everything was fine in the end as it could generate VS projects for me.

I don't know yet whether the final version of the Sigil compiler will have python in it. It would seem more efficient to have a separate program read in my EBNF and compile it to something easy to read in and have the Sigil compiler use that. I do however, like the idea of embedding Python into this project becuase that is just fun.

In order for Sigil's grammer to be able to parse everything correctly, it needs to be converted into an equivalent LR(1) grammer (for reasons that I don't fully understand, but has to do with Chomsky's Heirarchy).

In order to create a parser for an LR(1) grammer we need to generate the so called "First" and "Follow" sets for the grammer using the EBNF. There are plenty of descriptions of this algorithm in various resources, but I was unable to find any "good" pseudo-code so I am going to start writing some for any algorithms I encounter.
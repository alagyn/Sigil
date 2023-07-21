---
tag: devlog
---

Stardate - 7/10/23

Yeah, so... it's been over a year since the last devlog. My tendency to burnout struck yet again, combined with the fact that I successfully acquired gainful employment, which, believe it or not, is a real time-sink. That being said, Sigil has sat in the back of my mind and stewed for the last year and I've never given up the idea. I started working on it again a couple weeks ago and I'm finally getting around to writing the next log.

My last log didn't really do justice for what I had acccomplished prior to my *leave of absence*, so here is what I had done thus far: I had written a basic EBNF parser in python that could generate LR(1) and LALR(1) automata and I had begun writing some code samples and grammers for Sigil. My initial intention for this python code was for it to be embedded into the compiler and to load the grammar at runtime, but I foresee that being unfeasible and inefficient.

So, what I have done since then: I have rewritten a lot of the EBNF parser as I had made some poor design decisions and I had succumbed to the pre-optimization demon. I resolved a lot of that and fixed a number of issues. I also added a number of new unit tests which have been a big help with making sure everything works as expected. I have also mostly figured out the interface between the EBNF and the actual compiler. The python code generates a cpp source file containing the parse table and regex's for terminals which can then be compiled into the program thereby precomputing the grammer. In my *absence* I learned a lot about cmake from my day job and I was able to smoothly incorporate this into the build procedure so that the parse table is regenerated whenever the python code or the grammar file is edited.

As for development on the compiler, I have made good progress. I have a working scanner that also acts as a preprocessor filtering out comments. I think it is of rather standard construction, it reads a char at a time and returns "maximal-munch" tokens via regexes specified in the grammar file. I have also implemented the beginnings of the parser, which is a simple beast at the moment, and doesn't create syntax trees yet, but it does work to validate files.

Grammar-wise, I have written a lot of Sigil's grammar. Most of it has gone smoothly, but I am starting to run into parse table conflicts meaning that Sigil is not LALR(1) compatible without some grammar massaging. I have paused to look into another extension to LR(1) grammars called IELR(1) (Inadequacy-elimination LR) which is a method of conflict-resolution that should help smooth out some of the issues. The only obstacle is that the only good reference for the algorithm is the research paper it comes from which is a hard read and has been slow to process. Bison has an implementation of it but that codebase is very different from my own, so I am wary to try and learn anything from it.

[[009-big-fixes]]

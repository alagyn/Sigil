Stardate - 7/17/23

After thinking about how to get my parser to generate syntax trees, I realized it would
be easy to have my parse table generator make some functions for each rule that can be 
called whenever a reduce action occurs..... and I just reinvented Bison.

Not really though, in the process of fixing the lookahead thing from yesterday I had
been looking more into Flex/Bison since I wasn't making headway on the IELR stuff.
I had basically reimplemented my scanner/parser in Flex and Bison to see if it also had
the same limitations in it's LALR, or if IELR would even fix the problem. Comparing Bison
to my own implementation, I am coming to similar conclusions about how this thing should
operate.

So it seems like I'm going to implement my own entire compiler-compiler. I'll need to
come up with a better name than "ParseTableGen" for this thing and maybe look into
extracting it into its own project since technically it doesn't have to be Sigil
specific (other than how it is hard coded to skip comments...).

I've updated the grammar parser to be able to read in code blocks for each rule. This
required some major refactors to the preprocessor as it was quite naive in how it 
combined lines together. This whole combining lines thing is a bit bad to be honest,
it leads to poor error messages since you lose the real line number, but it could be
worse. But, I have robustified the preprocessor and it should be able to feasibly
handle more bad grammar formatting, like having more than one rule on a line. I also
improved error handling with a custom exception because just using RuntimeErrors is
bad practice and I knew better.

Another thing I've added to the grammar are "directives." For now there is just one
`%return` that is required and lets the user specify the return type of the reduce
functions. I don't know if I'll need anything more than that for now, but I can feasibly
add any special options I want.

I'm quite happy with my grammar files, I appreciate combining both scanner tokens
and parser rules into a single file. I also found the Flex/Bison files to be esoteric and
was trying to be *too* generic because it was supposed to spit out an **entire**
application. Ideally, my generator output should be essentially a library interface that
users can include, and can call functions to get whatever output they define in the 
grammar's code blocks. Granted, I may still need to implement a "prologue" section
but really, I am only envisioning needing to allow the user to include any header files
they need, but that can probably be done with a directive.

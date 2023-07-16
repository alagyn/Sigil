Stardate - 07/16/23

I found a bug in my LALR parse table generator, where I was using the follow
set to determine which symbols to place actions, but I should have been using
lookahead for the rule I was checking. This resolved all of the R/R conflicts
I had been fighting with.

Because of this, I feel that I may abandon the pursuit of implementing IELR table
generation until such a point that LALR is not powerful enough. This is primarily
due to the lack of complete psuedocode and the white paper being difficult to 
understand to my monkey brain.

I was also able to make improvements to the parse table generation. I eliminated the
first column, as it only ever contained an accept action at state 0. I could instead
just remove the starting the rule/start symbol and just accept the parse when I receive
a reduction according to rule 0, and just offsetting reduce lookups by 1.

My next steps will be to continue work on the parser in order to generate abstract
syntax trees.

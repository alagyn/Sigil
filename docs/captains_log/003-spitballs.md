# Spitballs
Here lies random musings on potential language features

### Garbage collection
All objects act as smart pointers
Simple to calculate

Issues:
    cycles are an issue, need a separate GC check

Could just fall back to C++ and put that responsiblity on the programmer

### regex
Be nice to have regex as a language construct

Have a specific delimiter to specify a string as regex so that we can eliminate the need for escape sequences
Obviously needs to be something uncommon in regex, or potentially a multi-char sequence
<"asdf">

have regex as a primitive type?

At a minimum, I know I want to have a way to represent raw strings using a special delimiter.

Perl has a raw regex syntax?

### built-in datatypes
I definitely want Lists, Dicts, and Sets to be built in.
I want to have standard array and deque implementations
Array/List
Both can be generated from list literals

Definitely going to need bools
As for numerics, I'm considering just having int/float?
Abstract away sizes maybe...

### Nested enums
enum ASDF
{
  A,
  B,
  enum Inner
  {
     C,
     D
  }
}

ASDF.A
ASDF.B
ASDF.Inner.C

Can declare a var as ASDF or specifically Inner

### kebab-case
Allow kebab case in names, underscores suck
this-name = asdf.potat()
this might be rough to implement in grammer

### aliases
`alias` keyword that functions like `typedef` but is a generic alias for any two names
Use to make short names for complex types
Use to make short names for deeply nested variables without make another reference?
Use to make nested module aliases
Stardate - 4/7/22

I have begun to write some code samples of what I want the language to look like. In doing so I have discovered some things that I want to try to implement.

First of all, typing. I knew from the beginning that Sigil would be a strongly typed language, and I wouldn't change my mind about that. But, I appreciate the cleanliness of python allowing me to simply create a variable with a name and not needing to write out a potentially long and complicated type specification like in Java or C++. Therefore, I want to make typing optional for variables. Essentially, we would be assuming that you are using the "auto" keyword from c++ which leaves it up to the compiler to infer the type unless you have explicitly specified one. However, this only applies to variables. Function arguments must be typed to ensure that arguments can be type checked at compile time. I may also be able to make the type optional for func arguments if a default value is given, unless null is given as the default, in which case the type must still be specified. I think that function return types will always be required, although I guess I could try to infer from the return statements but that seems more difficult than checking every return type to the specified one.
Since function return types will always be known at compile time, all variable types can be inferred at compile time making typing errors visible at compile time. In other words, Sigil is strongly typed without needing every variable type to be declared.

I was also planning on making lists be a language construct, and I think the way I'm going to do it is that Lists can only have one datatype contained in them so that it is easy to infer the type.
I think I will also want to implement Tuples, which can have more than one datatype, but the exact number of each must be known at compile time.
I might have the type templating be with square brackets like python's typing package, but if this interferes with the array operator I may just user corner brackets.
List[int] vs List<int>
List[int] x = [0, 1, 2, 3, 4];
Tuple[int, String, Person] x = (0, "potato", Person("Jim));

I also like the readability of python's branch/loop statements in that they don't require parentheses around the expressions.
if(x == 0) {}
vs
if x == 0:
I do not, however, like python's lack of brackets. Creating code blocks using only indentation is... not great. Therefore, I want to combine the two approaches:
if x == 0 {}

IF_STMT = "if" EXPR BLOCK

For simple ifs, you can omit the brackets for a "then" and an expression plus additionally an else.

IF_STMT = "if" EXPR "then" EXPR "else" EXPR ";"


I want to apply this to all control statements, ifs, fors, whiles, etc. 

I also want to bring in python-like for-in loops
for x in list {}
x's type can be inferred from the list

I also want to use this syntax for c++ style for loops, but using keywords
for x = 0 while x < 25 then ++x {}
Not sure if I like "then" as the keyword there, but it is at least it doesn't add a new keyword. "do" doesn't quite work there at least.

WHILE_STMT = "while" EXPR BLOCK
DO_WHILE_STMT = "do" BLOCK "while" EXPR ";"

As for classes, I think they will be pretty similar to C++, just with some extra access modifiers. First of all, I like that in c++ I can use a label to define multiple things under the same access modifier (unlike java, where you have to specify public/private/.. for every declaration). That being said, I think it can be improved. Specifically, I want to use brackets to surround everything in an access group, instead of just being in the same group until we hit another label.
public
{
  int x;
  static float z;
}

But, I think I won't allow functions to be defined in these blocks (because of the new accesses I will define momentarily) meaning they will still have to explicitly state their access.

Now, as for the access modifiers, I have come up with 5.
public: The same as normal, can be freely accessed from outside the class, as well as subclasses
private: The same as normal, can be accessed ONLY from within this class, i.e. not including subclasses
shared: The same as "protected" in c++, i.e. accessed only by this class and subclasses
readonly: Can be freely accessed from inside this class, but acts in a "const" manner from outside
common: Can be freely accessed from inside this class and subclasses, but is readonly from outside
	Not sure about the keyword for this one

I need to sit down and clearly define "const" as it has strange side-effects in c++

Functions

Class constructors will be similar to c++ and java, i.e. they are functions with no return type and named the same as the class.

Classes will otherwise act exactly the same as java classes. Classes methods will have an implicit "this" reference, as opposed to python/JS which explicity specify "self" as the first argument.

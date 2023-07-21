---
tag: devlog, hermes
---

Stardate - 7/20/23

I have extracted the parser generator into it's own repo now.
I have dubbed it [__Hermes__](https://github.com/alagyn/Hermes), the Greek god of language and the inventor of writing. It still has some work needed, I need to finish the code generation step to allow for the user code to be registered. I also need to update the parser to use said functions and make cmake turn Hermes into a library that can be linked against for the main Sigil compiler to use.

In other news, I was introduced to a new tool called [Obsidian](https://obsidian.md/) which is everything I had wanted one of my old projects "Captain's Log" to be (this devlog was the original inspiration for that too). It has a ton of cool things to play with (I especially like the canvas plugin). I expect that it will come in super handy everywhere.

### TODO
- [ ] Grammer parsing
    - [ ] PP needs to ignore code blocks
    - [ ] Eliminate "combine lines" and use better RE iteration
- [ ] Fix python unit tests
- [ ] Rule function generation
    - [x] Function list
    - [ ] code preprocess
- [ ] Parser updates
    - [x] on reduction, call rule func
    - [ ] better lib api
    - [ ] cmake

### Hermes other things:
If I want to support multiple langs, need to be able to gen multiple parse table files with different namespaces. Might be difficult

Preprocess user code in ebnf to auto replace `$x` with a conversion to either a HERMES_RETURN, or a string if it is a terminal, where x is the symbol idx
or use named `$name`

Add a comment directive to designate line and block comment prefixes/suffixes
# Compiler Principles

SSDLUT 2019 Spring Semester - Compiler Principles Total 8 Assignments

## Assignment I - Simple Lexical Analyzer

### 1 Programming Language & IDE

C++17

JetBrain CLion

* Be Sure to Set Your CWD (Current Working Directory) to Your Project Root Directory

### 2 Assignment Briefing

Process Source Code Into Token Stream

#### * Example

Input Source Code

1. Type I
> x := 5;
> if ( x > 0 )
> then
> x := 2 * x + 1 / 3;
> else
> x := 2 / x;
> #

2. Type II
> x:=5;
> if(x>0)
> then
> x:=2*x+1/3;
> else
> x:=2/x;
> #

3. Type III
> x:=500;
> if(x>0)
> then
> x:=20*x+15/3;
> else
> x:=2/x;
> #

Output Generated Token Stream
> <10,x> <18,:=> <11,5> <26,;> 
> <2,if> <27,(> <10,x> <23,>> <11,0> <28,)> 
> <3,then> 
> <10,x> <18,:=> <11,2> <15,*> <10,x> <13,+> <11,1> <16,/> <11,3> <26,;> 
> <4,else> 
> <10,x> <18,:=> <11,2> <16,/> <10,x> <26,;> 
> <0,#> 

## Assignment II

### 1 Programming Language & Tool

Flex (*The Fast Lexical Analyzer*) (Linux Version)

### 2 Assignment Briefing

1. Write A ***Lex Source Program*** (.l File), e.g. example.l
2. Generate ***.C File*** Based On The Lex Source Program Using ***Flex***
3. Compile The C Program Into ***.out File*** or ***.exe File***
4. Test The Generated Simple Lexical Analyzer

* Implement 2 Main Functions
  * Translate Your Student ID Number In The Import File --> Your Name / Nickname
  * Count ID & Chars In The Import File

#### * Example

Import Source File

> 201693072 Hello World
> Flex (The Fast Lexical Analyzer)
> Flex is a fast lexical analyser generator.
> It is a tool for generating programs that perform pattern-matching on text.
> Flex is a free (but non-GNU) implementation of the original Unix lex program.
> The project repository is now hosted at github: https://github.com/westes/flex.
> 201693072

Output Text

> Karaya_12 Hello World
> Flex (The Fast Lexical Analyzer)
> Flex is a fast lexical analyser generator.
> It is a tool for generating programs that perform pattern-matching on text.
> Flex is a free (but non-GNU) implementation of the original Unix lex program.
> The project repository is now hosted at github: https://github.com/westes/flex.
> Karaya_12
> Number of Lines: %d, Number of Chars: %d

### 3 Terminal Commands (Linux)

File Stream

example.l (Flex Source Program) --> lex.yy.c (Flex Generated C Program) --> example.out (Complied C File) --> Import Text File --> Display Final Result In CLI

```
$ flex example.l
$ gcc -o example lex.yy.c -lfl
$ ./example < Text.txt
```

## Assignment III

### 1 Programming Language & IDE

C++17

JetBrain CLion

* Be Sure to Set Your CWD (Current Working Directory) to Your Project Root Directory

### 2 Assignment Briefing

1. Calculate The FIRST & FOLLOW Sets of Given Target Grammar

* Be Sure to Check Out The Principles of FIRST & FOLLOW Sets First

  https://www.jambe.co.nz/UNI/FirstAndFollowSets.html

#### * Example

- Using '#' Instead of 'ε', It Makes Life Easier.

Test Set I

> E -> TM
> M -> +TM | #
> T -> FN
> N -> *FN | #
> F -> (E) | a

FIRST Set I

> FIRST(E) = { (, a }
> FIRST(M) = { ε, + }
> FIRST(T) = { (, a }
> FIRST(N) = { ε, * }
> FIRST(F) = { (, a }

FOLLOW Set I

> FOLLOW(E) = { $, ) }
> FOLLOW(M) = { $, ) }
> FOLLOW(T) = { $, ), + }
> FOLLOW(N) = { $, ), + }
> FOLLOW(F) = { $, ), *, + }

Test Set II

> A -> BCDE
> B -> aBA | #
> C -> F | #
> D -> b | c | #
> E -> e | #
> F -> d | #

FIRST Set II

> FIRST(A) = { ε, a, b, c, d, e }
> FIRST(B) = { ε, a }
> FIRST(C) = { ε, d }
> FIRST(D) = { ε, b, c }
> FIRST(E) = { ε, e }
> FIRST(F) = { ε, d }

FOLLOW Set II

> FOLLOW(A) = { $, a, b, c, d, e }
> FOLLOW(B) = { $, a, b, c, d, e }
> FOLLOW(C) = { $, a, b, c, d, e }
> FOLLOW(D) = { $, a, b, c, d, e }
> FOLLOW(E) = { $, a, b, c, d, e }
> FOLLOW(F) = { $, a, b, c, d, e }

### 3 Assumptions

Reasonable Assumptions Make Life Easier ~~~

1. *Epsilon ε is represented by ‘#’*
2. *Grammar is not left recursive*
3. *Only upper case letters are Non-Terminals(NT), thus everything else is a Terminal(T)*

### 4 Previous Issues & Current Solutions

1. Currently Only Support Split Productions
   ```
   A -> aAb or A -> aAb | ε
   ```
   Do NOT Support
   ```
   A -> aAb | cBd or A -> aAb | cBd | ε
   ```
   You'll Have to Change Them to
   ```
   A -> aAb
   A -> cBd
   A -> ε
   ```
   
   * Solution - Split Function Added Before Running The Actual FIRST & FOLLOW Set Function.
   
2. Currently Only Support Limited CFG Grammar Rule Types
   e.g. Following Grammar Not Supported

   ```
   A -> BCDE
   B -> aBA | #
   C -> F | #
   D -> b | c | #
   E -> e | #
   F -> d | #
   ```

   * Solution - A C++ STL Set Has Been Added to Stop Infinite Loop In FOLLOW Set Calculation Procedure.

## Assignment IV

### 1 Programming Language & IDE

C++17

JetBrain CLion

- Be Sure to Set Your CWD (Current Working Directory) to Your Project Root Directory

### 2 Assignment Briefing

1. Generate LL(1) Grammar Parsing Table Based On Previous FIRST & FOLLOW Set Result
2. Top-Down Non-Recursive Predictive Parsing
3. Top-Down Recursive Descent Parsing

#### * Example

Test Set

> E -> TM
> M -> +TM | #
> T -> FN
> N -> *FN | #
> F -> (E) | a

FIRST Set

> FIRST(E) = { (, a }
> FIRST(M) = { ε, + }
> FIRST(T) = { (, a }
> FIRST(N) = { ε, * }
> FIRST(F) = { (, a }

FOLLOW Set

> FOLLOW(E) = { $, ) }
> FOLLOW(M) = { $, ) }
> FOLLOW(T) = { $, ), + }
> FOLLOW(N) = { $, ), + }
> FOLLOW(F) = { $, ), *, + }

Parsing Table

* Number Presents The Production Which Generates The Corresponding FIRST Set

>   $ ( ) * + a
> E / 0 / / / 0 
> M # / # / 1 / 
> T / 3 / / / 3 
> N # / # 4 # / 
> F / 6 / / / 7 

Custom Input

> a+a*a$
> aa*a$
> a**a$
> (a+a$

Final LL(1) Grammar Parsing Result

> E -> TM
> T -> FN
> F -> a
> N -> #
> M -> +TM
> T -> FN
> F -> a
> N -> *FN
> F -> a
> N -> #
> M -> #

## Assignment V

### 1 Programming Language & IDE

C++17

JetBrain CLion

- Be Sure to Set Your CWD (Current Working Directory) to Your Project Root Directory

### 2 Assignment Briefing

1. Generate LR/SLR Grammar Parsing Table Based On Grammar Closure & FOLLOW Set Result
2. Bottom-Up LR Shift / Reduce Parsing

* Using Embedded LR Parsing Table

#### * Example

Test Set

> E -> E+T | T
> T -> T*F | F
> F -> (E) | a

FOLLOW Set

> FOLLOW(E) = { $, ), + }
> FOLLOW(T) = { $, ), *, + }
> FOLLOW(F) = { $, ), *, + }

Custom Input

> a*a+a$ --> No Error
> a*+a+a$ --> '+' Will be Discarded

Parsing Table, Final Result & Detailed LR Parsing Principles

> Be Sure to Check Out This PDF - http://www.cse.aucegypt.edu/~rafea/CSCE447/slides/LR%20ParsingNEW.pdf

## Assignment VI

### 1 Programming Language & IDE

C++17

JetBrain CLion

- Be Sure to Set Your CWD (Current Working Directory) to Your Project Root Directory

### 2 Assignment Briefing

1. Semantic Analysis Based On Previous LR Parser
2. Be Sure to Check Out How Semantic Rules & Actions Work First

* Using Embedded LR Parsing Table & Semantic Rules, Actions

#### * Example

Test Set

> E -> E+T | T
> T -> T*F | F
> F -> (E) | a

FOLLOW Set

> FOLLOW(E) = { $, ), + }
> FOLLOW(T) = { $, ), *, + }
> FOLLOW(F) = { $, ), *, + }

Custom Input

> 5+3+8x2$

Result

> 24.0000 (float Value)
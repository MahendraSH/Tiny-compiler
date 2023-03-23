# Tiny compiler
----
## Compiler build on top of C compiler using C language.<br />
## BOOK : Compiler Construction: Principles and Practice - Kenneth C. Louden 
scanner and parser for tiny lang
### tiny scanner using c language
-----
-----
#### Sample program
```
{ Sample program
  in TINY language -
  computes factorial
}
read x; { input an integer }
if 0 < x then { don't compute if x <= 0 }
  fact := 1;
  repeat
    fact := fact * x;
    x := x - 1
  until x = 0;
  write fact  { output factorial of x }
end
```
---------
#### output:
```
TINY COMPILATION: SAMPLE.TNY
   1: { Sample program
   2:   in TINY language -
   3:   computes factorial
   4: }
   5: read x; { input an integer }
        5: reserved word: read
        5: ID, name= x
        5: ;
   6: if 0 < x then { don't compute if x <= 0 }
        6: reserved word: if
        6: NUM, val= 0
        6: <
        6: ID, name= x
        6: reserved word: then
   7:   fact := 1;
        7: ID, name= fact
        7: :=
        7: NUM, val= 1
        7: ;
   8:   repeat
        8: reserved word: repeat
   9:     fact := fact * x;
        9: ID, name= fact
        9: :=
        12: ID, name= fact
  13: end
        13: reserved word: end
        14: EOF
        ```
----------------------------------------------------------------
----------------------------------------------------------------
### tiny Parser using c language

####  Sample program 1 

```
{ Sample program
  in TINY language -
  computes add two numbers
}
read x; { input an integer }
read y;
sum := x + y;
write sum


```
----------------------------------------------------------------
#### output:
```TINY COMPILATION: SAMPLE.TNY
   1: { Sample program
   2:   in TINY language -
   3:   computes add two numbers
   4: }
   5: read x; { input an integer }
   6: read y;
   7: sum := x + y;
   8: write sum
   9:

Syntax tree:
  Read: x
  Read: y
  Assign to: sum
    Op: +
      Id: x
      Id: y
  Write
    Id: sum

```
----------------------------------------------------------------
----------------------------------------------------------------
#### Sample program 2
```
{ Sample program
  in TINY language -
  computes add two numbers
}
read x; { input an integer }
read y;
sum := x + y;
write sum
end 
```
----------------------------------------------------------------
#### output :

```

TINY COMPILATION: SAMPLE.TNY
   1: { Sample program
   2:   in TINY language -
   3:   computes add two numbers
   4: }
   5: read x; { input an integer }
   6: read y;
   7: sum := x + y;
   8: write sum
   9: end

>>> Syntax error at line 9: Code ends before file

Syntax tree:
  Read: x
  Read: y
  Assign to: sum
    Op: +
      Id: x
      Id: y
  Write
    Id: sum

```

# Tiny-compiler
scanner and parser for tiny lang
### tiny scanner using c progaming
-----
-----
####Sample program
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

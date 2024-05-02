# Polynomial Calculator
This is a basic polynomial calculator built to handle addition, subtraction, and multiplication operations on polynomials. Polynomials are read from command line or from text files. Polynomials can be stored with a variable name, arithmetic operations can be performed on them. Polynomials can be written to command line or to text files.

x is used as polynomial variable.

### Creating/Assigning polynomials
`polynomial-name := literal-polynomial` for literal polynomials without any brackets within them
`polynomial-name = named-polynomial-expression` for any polynomial expression

polynomials without any named-polynomials within them are called literal polynomials.
Ex: 2x+3, 4x^2 -3x +12, x^3 -9 are considered literal polynomials.

An expression like a +2x +b is not considered as literal polynomial, because it contains named-polynomials a and b, which may be stored in program.

You can always use "=" for any type of expression. You should use ":=" for literal polynomials without any brackets within them.

":=" is efficient and provides better error messages compared to "=". However, whether you use "=" or ":=", the result will be the same.

The read polynomial will be printed immediately following the given input. For example:
```text
a := x^3 - 3x^2 +5
a =
     x^3 -3x^2 +5
```

To avoid printing the polynomial, you can use a semicolon at the end:
```text
a := x^3 -3x^2 +5;

```
a will be stored in the memory

Input format of each term in polynomial `<sign>(+/-) <co-eff>x^<power> …`
You can ignore writing co-eff if it is one, similarly you can ignore writing "^1" if power is one, and for constant terms, you can simply write the co-eff.
Both x and X will be considered as variable of polynomial.
 
While there can be white spaces between signs and terms, but white spaces should be avoided within the terms.
For Example:
`x^3 - 3x^2 + 1`and `x^2 +1` are valid.
`x ^3- 3 x^2 +1` and `x ^ 2 +1` are invalid.
Error messages will be printed for invalid polynomials.
```text
a := 3x^ 2 +4
ERROR: Could not parse the polynomial:
         3x^ 2 +4
            ^
```
The position where the error occurred will be indicated by "^"
<br>

```text
a := 3x +2;
b := 4x^2 -x;
c := 5x;
```
All these three variables can be defined in a single line, separated by semicolon
```text
a := 3x +2; b := 4x^2 -x; c := 5x;
```
a, b, c will be stored in the memory

#### Rules for Naming Polynomial Variables
- Polynomial names can include alphabets, numbers, and underscores, but should not contain any special symbols or spaces.
- They shouldn’t start with a number.
- While the characters x and X is allowed in the name of the polynomial, the polynomial name should not consist solely of either “x” or “X”. In other words, the variable name should be more descriptive or specific than just being named either “x” or “X”.

Note: *The program uses the name “ans” to store unnamed expressions. While you can still use it, please note that it may get overwritten by the program.*

### Arithmetic Operations on Saved Polynomials
```text
a:=3x; b:=5x^2-2; c:=x^3-4x;
k = a+b+c
k =
     x^3 +5x^2 -x -2
```

You can avoid printing the result by adding a semicolon at the end
```text
a:=3x; b:=5x^2-2; c:=x^3-4x;
k = a+b+c;
```
If you do not provide any name for the result, result will be stored with name "ans"
```text
a := 3x; b := x^2-4;
a+b
ans =
       x^2 +3x -4
```
<br>

```text
a := 3x; b := 2x^2 -3; c := x-1; d := x^3 -x^2 +2;
```
```text
a*b+c
ans =
       6x^3 -8x -1
```
You can use literal polynomials also
```text
a*x + c
ans =
       3x^2 +x -1
```
You can use brackets
```text
(a+c)*4 + b*(x^2 +x -1)
ans =
       2x^4 +2x^3 -5x^2 +13x -1
```

**Error Messages**
```text
a+b-k
Error occurred while parsing expression:
Unknown element: k
```
```text
a+x^ 3 -b
Error occurred while parsing expression:
Unknown element: x^
```

### Display Polynomials (#)
Display command starts with "#"
Syntax: "# poly-name1 poly-name2 ..."
These polynomials (poly-name1, poly-name2 ...) will be printed to the screen
```text
# a b c d
           a = 3x
           b = 2x^2 -3
           c = x -1
           d = x^3 -x^2 +2
```

### Delete Polynomials (!)
Delete command starts with "!"
Syntax: "! poly-name1 poly-name2 ..."
These polynomials (poly-name1, poly-name2 ...) will be deleted from the memory
```text
! d c k
ERROR: Unknown polynomial k
```
Known polynomials will be deleted. This error message will be printed if we try to delete unknown polynomials.

### Display Entire Database (## or $)
command - "##" or "\$"
Syntax: ## or $
All the known polynomials will be printed
```text
$
           a = 3x
           b = 2x^2 -3
         ans = 2x^4 +2x^3 -5x^2 +13x -1
```
```text
##
           a = 3x
           b = 2x^2 -3
         ans = 2x^4 +2x^3 -5x^2 +13x -1
```
for empty database
```text
$
Database is empty
```

### Delete Entire Database (!!)
command - "!!"
Syntax: !!
You will be prompted again to confirm the deletion of the entire database.
```text
!!
Delete entire database?[Y/N]: y
Cleared entire database
```
```text
!!
Delete entire database?[Y/N]: n
Deletion Cancelled. Database remains unchanged
```

### Write Polynomials to a file (@w)
command - "@w"
Syntax: @w filename
```text
@w out.txt
Polynomials are successfully written to the file
```
out.txt
```text
           a := 3x 
           b := 4x^2 -3x +1 
           c := 4x^3 -7x^2 +4x -1 
           d := 4x^3 -6x^2 +7x -4 
```
If database is empty
out.txt
```text
NULL
```

### Read Polynomials from a file (@r)
command - "@r"
Syntax: @r filename

sample.txt
```text
           a := 3x 
           b := 4x^2 -3x +1 
           c := 4x^3 -7x^2 +4x -1 
           d := 4x^3 -6x^2 +7x -4 
```
<br>

```text
@r sample.txt
Polynomials read from the file: a b c d
```
```text
$
           a = 3x
           b = 4x^2 -3x +1
           c = 4x^3 -7x^2 +4x -1
           d = 4x^3 -6x^2 +7x -4
```

Note: To read polynomials from file, they must be in the literal polynomial assignment format `polylnomial-name := polynomial`. Otherwise that line will be ignored.

### Exit command (*)
command - "\*"
Syntax: "\*"
```text
*
The program has terminated
```

### Rename Polynomials (^)
rename command starts with "^"
Syntax: "^ previous-name updated-name"
```text
a := 3x-2; b := x^2 -4;
^ a k
k =
     3x -2
```
Use semicolon to avoid printing
```text
^ a k
```
**Error Messages**
```text
^ d k;
ERROR: Unknown Polynomial d
```
```text
^ k b;
Polynomial b already exists in database
```

### Polynomial Input Command (>)
This command was used in the previous versions. Now we have ":=" and "="
Input command starts with ">"
syntax: "> poly-name1 poly-name2 ..."
Program will keep asking you to enter the polynomial until you enter a valid polynomial. It will print the read polynomial.
```text
> a b
Enter the polynomial a: x^ 2 +3
ERROR: Could not parse the polynomial:
        x^ 2 +3
          ^
Enter the polynomial a: x^2 +3
Entered polynomial: a = x^2 +3
Enter the polynomial b: x ^3 -4
ERROR: Could not parse the polynomial:
        x ^3 -4
          ^
Enter the polynomial b: x^3 -4
Entered polynomial: b = x^3 -4
```

### Multiple Commands in Single Line
You can enter multiple commands in a single line by separating them with with semicolon ";"

Example: `a := x^2 -3x; b = a; ! a; $ `
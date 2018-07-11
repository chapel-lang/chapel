use intChoicesHelp, intChoicesTypes;

/* This test is similar to intChoices.chpl except that it
   'accidentally' defines the enums twice -- once here and once in the
   intChoicesTypes module.  Defining a symbol twice is not a bug and
   the local one will shadow any external one.  The surprising (but
   correct) thing is that the functions resolve differently than in
   intChoices.chpl because the function definitions themselves are
   referring to the enums in intChoicesTypes, which are distinct from
   these local ones.  So all the enum routines resolve to the integer
   versions instead, correctly.

   This suggests that it might be useful to have a warning about one
   module shadowing another's symbols; or at least types?
*/

config param tryErrors = false;

enum color {
  red = 0,
  white = 1,
  blue = 2
};

var aColor: color = color.red;

param RED = 0,
      WHITE = 1,
      BLUE = 2;

var Red = 0,
    White = 1,
    Blue = 2;

var i8 = 8:int(8),
    i16 = 16:int(16),
    i32 = 32: int(32),
    i64 = 64: int(64);

param I8: int(8) = 8,
      I16: int(16) = 16,
      I32: int(32) = 32,
      I64: int(64) = 64;

enum sizes {
  small = 126,
  big = 131072
};

var aSize: sizes = sizes.small;
var anotherSize: sizes = sizes.big;

param SMALL = 126,
      BIG = 131072;

var Small = 126,
    Big = 131072;

writeln("intOrEnum variants");
if !tryErrors then writeln(); else intOrEnum(color.red);
if !tryErrors then writeln(); else intOrEnum(aColor);
intOrEnum(RED);
intOrEnum(Red);
intOrEnum(i8);
intOrEnum(i16);
intOrEnum(i32);
intOrEnum(i64);
intOrEnum(I8);
intOrEnum(I16);
intOrEnum(I32);
intOrEnum(I64);
if !tryErrors then writeln(); else intOrEnum(sizes.small);
if !tryErrors then writeln(); else intOrEnum(sizes.big);
if !tryErrors then writeln(); else intOrEnum(aSize);
if !tryErrors then writeln(); else intOrEnum(anotherSize);
intOrEnum(SMALL);
intOrEnum(BIG);
intOrEnum(Small);
intOrEnum(Big);
intOrEnum(126);
intOrEnum(131072);
writeln();

use IO;

type newInt=  int;
var  myInt:   newInt;

type newFloat=    real;
var  myFloat:     newFloat;

type newBoolean=  bool;
var  myBoolean:   newBoolean;

type newComplex=  complex;
var  myComplex:   newComplex;

write("Enter an int: ");
read(myInt);
writeln("You entered ", myInt);

write("Enter a real: ");
read(myFloat);
writeln("You entered ", myFloat);

write("Enter a bool: ");
read(myBoolean);
writeln("You entered ", myBoolean);

write("Enter a complex number: ");
read(myComplex);
writeln("You entered ", myComplex);

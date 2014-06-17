/* The call to foo below is ambiguous. (2,(3,4)) could match either (b,(c,d))
 * with b,c, and d getting int values, or (b, c) with b getting an int value
 * and c getting the tuple (3,4). Check that this is an error.
 */

proc foo(a: int, (b, (c, d))) {
  writeln("One normal arg, one nested tuple arg.");
}

proc foo(a: int, (b, c)) {
  writeln("One int arg, one two-tuple arg.");
}

foo(1, (2,(3,4)));

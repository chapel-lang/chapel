//
// The point of this test is that 'type select' is an unnecessary
// concept.  We should just use select on type expressions.  I was
// going to make this a feature request future, but it turns out
// that it already works.  Nice!
//

config param useInt = true;

proc pickType() type {
  if useInt then 
    return int;
  else 
    return real;
}

var x: pickType();

write("x is ");
select (x.type) {
 when int do writeln("int!");
 when real do writeln("real!");
 otherwise writeln("confused!");
}

write("pickType() returns ");
select (pickType()) {
 when int do writeln("int!");
 when real do writeln("real!");
 otherwise writeln("confused!");
}

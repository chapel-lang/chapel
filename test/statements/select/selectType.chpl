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

config param useInt = true;

proc pickType() type {
  if useInt then 
    return int;
  else 
    return real;
}

var x, y: pickType();

write("x is ");
type select x {
  when int do writeln("int!");
  when real do writeln("real!");
  otherwise writeln("confused!");
}

write("y is ");
type select y {
  when int do writeln("int!");
  when real do writeln("real!");
  otherwise writeln("confused!");
}

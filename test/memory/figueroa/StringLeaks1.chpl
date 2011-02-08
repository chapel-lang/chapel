use Memory;

config const n = 1;

proc foo () {
  var s1 = "a string",
      s2 = s1,
      s3 = "another string",
      s4 = s3,
      s5 = "Good bye",
      s6 = s5,
      s7 = "Nos vemos",
      s8 = s7,
      s9 = "Au revoir",
      s0 = s9;
  if n < 0 then
    writeln(s2);
  else writeln(s1);
  if n < 0 {
    if n < -1 then
      writeln(s4);
    else writeln(s3);
  }
  select n {
    when 0 do writeln(s6);
    when 1 do writeln(s8);
    when 2 do writeln(s0);
  }
}

var m1 = memoryUsed();
foo();
var m2 = memoryUsed();

writeln("Amount of leaked memory after calling foo(): ", m2:int - m1:int);

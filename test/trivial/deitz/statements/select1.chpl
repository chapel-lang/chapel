var x : int = 12;

writeln(x);

select x {
  when 3, 4, 2 + 8 do writeln("failure");
  when 8 + 4 { writeln("success"); }
  otherwise writeln("otherwise");
}

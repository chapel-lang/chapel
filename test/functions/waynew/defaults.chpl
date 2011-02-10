// Test how the default value determines the arg type. 

proc jam( a = true, b:real = 4, c = 1.234) {
  writeln();
  writeln( "a = ", a);
  writeln( "b = ", b);
  writeln( "c = ", c);
}

jam();
jam( false);
jam( true);
jam( false, 1);
jam( false, 1, 2.3);
jam( false, 2.8);
jam( false, 2, 8);

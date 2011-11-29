proc f(x:int, y:int, z:int) {

  // Replace: y=x+1 and z=2(x+1)
  proc g(y:int) {
    var z = 2*y;
    writeln("x=",x);
    writeln("y=",y);
    writeln("z=",z);
  }

  writeln("x=",x);
  writeln("y=",y);
  writeln("z=",z);
  writeln();

  g(x+1);
}

f(1, 3, 5);

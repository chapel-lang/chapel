class C {
  var x: int = 1;

  def x var {
    if !setter && x < 0 then
      halt("x accessed when negative");
    return x;
  }
}

var myC = new C(x = 2);
writeln("myC is: ", myC);

myC.x = 3;
writeln("myC is: ", myC);

myC.x = -1;
writeln("myC is: ", myC);

class C {
  var x: int = 1;

  def =x(newx: int) {
    if (newx < 0) {
      halt("Can only assign C.x non-negative integers!");
    }
    x = newx;
  }
}

var myC = C(x = 2);
writeln("myC is: ", myC);

myC.x = 3;
writeln("myC is: ", myC);

myC.x = -1;
writeln("myC is: ", myC);

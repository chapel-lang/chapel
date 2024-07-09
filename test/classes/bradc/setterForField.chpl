class C {
  var x_field: int = 1;

  proc init(x: int) {
    this.x_field = x;
  }

  proc x ref {
    return x_field;
  }
  proc x {
    if x_field < 0 then
      halt("x accessed when negative");
    return x_field;
  }

}

var ownMyC = new owned C(x = 2);
var myC = ownMyC.borrow();
writeln("myC is: ", myC);

myC.x = 3;
writeln("myC is: ", myC);

myC.x = -1;
writeln("myC is: ", myC);
writeln("myC.x is: ", myC.x);

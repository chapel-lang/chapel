class C {
  var a: atomic int;

  proc init(x: atomic int) {  // x should be passed by ref by default
    a = x;
    x.add(1);
  }
}

var b: atomic int;

var myC = new owned C(b);
writeln("b is: ", b.read());
writeln("myC is: ", myC);

    

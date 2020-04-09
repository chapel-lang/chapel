class C {
  proc foo(i : int, j : int)
    return i**2 + j**2;
}

var c = new owned C();

var a1: [1..3] int = (1, 2, 3);
var a2: [1..3] int = (4, 5, 6);
writeln(c.foo(a1, a2));

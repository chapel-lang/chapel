
// This test exists to ensure that we do not encounter weird scoping issues
// when a top-level type has a constructor and a nested type has an
// initializer. This case did not 'just work' when initializers were enabled
// for nested types and required compiler changes.

class Node {
  var x : int;
}

var n = new unmanaged Node();
writeln(n);

class C {
  var n : unmanaged Node = new unmanaged Node();
  class Node {
    var x : int;
  }
}

var c = new unmanaged C();
writeln(c);

delete n;
delete c.n;
delete c;

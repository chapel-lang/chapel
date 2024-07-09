class foo {
  var x : int;

  proc setxlo(i : int) {
    x = i-1;
  }

  proc setxhi(i : int) {
    x = i+1;
  }

  proc getx() : int {
    return x;
  }
}

var ownF = new owned foo();
var f : borrowed foo = ownF.borrow();

writeln(f.x);
writeln(f.getx());

f.setxhi(3);
writeln(f.x);
writeln(f.getx());

f.setxlo(3);
writeln(f.x);
writeln(f.getx());

f.x = 5;
writeln(f.x);
writeln(f.getx());

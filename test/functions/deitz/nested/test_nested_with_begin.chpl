var s: sync bool;

proc foo() {
  var i: int = 2;

  proc bar(x) {
    writeln(x);
  }

  begin {
    s;
    bar(i);
  }
}

proc goo() {
  var i: int = 1;
  writeln(i);
}

foo();
goo();
s = true;

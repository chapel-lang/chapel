var s: sync bool;

proc foo() {
  var i: int = 2;

  proc bar() {
    writeln(i);
  }

  begin {
    s;
    bar();
  }
}

proc goo() {
  var i: int = 1;
  writeln(i);
}

foo();
goo();
s = true;

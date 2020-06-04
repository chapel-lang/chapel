var s: sync bool;
proc ensureDefaultInit(arg) { }
ensureDefaultInit(s); // make sure s is default-initialized empty

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

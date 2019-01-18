class foo {
  var i: int;

  proc blah() ref {
    return i;
  }

  proc blah() {
    return 3;
  }
}

var f = new unmanaged foo();

f.blah() = 4;

writeln(f.i);
writeln(f.blah());

delete f;

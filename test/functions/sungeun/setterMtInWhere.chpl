
class foo {
  var i: int;
  proc blah() ref where setter {
    return i;
  }
  proc blah() ref where !setter {
    return 3;
  }
}

var f = new foo();
f.blah() = 4;

writeln(f.i);
writeln(f.blah());
